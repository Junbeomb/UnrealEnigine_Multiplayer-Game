// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "ArenaBattle.h"
#include "Kismet/KismetMathLibrary.h"

UABCharacterMovementComponent::UABCharacterMovementComponent()
{
	bPressedTeleport = false;
	bDidTeleport = false;
	TeleportOffset = 600.f;
	TeleportCooltime = 3.f;

	bPressedDodge = false;
	bDidDodge = false;
	bIsDodge = false;
	DodgeOffset = 200.f;
	DodgeCooltime = 0.2f;

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloatRef(TEXT("/Script/Engine.CurveFloat'/Game/ArenaBattle/Animation/FC_Dodge.FC_Dodge'"));
	if (nullptr != CurveFloatRef.Object)
	{
		DodgeCurve = CurveFloatRef.Object;
	}
}

void UABCharacterMovementComponent::SetTeleportCommand()
{
	bPressedTeleport = true;
}

void UABCharacterMovementComponent::ABTeleport()
{
	if (CharacterOwner) {
		FVector TargetLocation = CharacterOwner->GetActorLocation() + CharacterOwner->GetActorForwardVector() * TeleportOffset;
		//CharacterOwner->TeleportTo(TargetLocation, CharacterOwner->GetActorRotation(), false, true);
		CharacterOwner->GetCharacterMovement()->AddImpulse(TargetLocation, true);
		bDidTeleport = true;

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&] 
			{
				bDidTeleport = false;
			}
		), TeleportCooltime, false, -1.f);
	}
}

void UABCharacterMovementComponent::SetDodgeCommand()
{
	bPressedDodge = true;
}

void UABCharacterMovementComponent::ABDodge()
{
	if (CharacterOwner) {
		DodgeTargetLocation = GetActorLocation() + CharacterOwner->GetActorForwardVector() * DodgeOffset;
		DodgeStartLocation = GetActorLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(DodgeStartLocation, DodgeTargetLocation);
		CharacterOwner->SetActorRotation(LookAtRotation);

		MoveTL = NewObject<UTimelineComponent>(this, FName("MoveTL"));
		MoveTL->RegisterComponent();
		MoveTLCallback.BindUFunction(this, FName("SetMoveTLUpdate"));
		MoveTLFinishedCallback.BindUFunction(this, FName("MoveTLFinished"));
		UE_LOG(LogTemp, Warning, TEXT("ABDodge"));
		if (DodgeCurve) {
			MoveTL->AddInterpFloat(DodgeCurve, MoveTLCallback, FName("MoveValue"));
			MoveTL->SetTimelineFinishedFunc(MoveTLFinishedCallback);
			MoveTL->PlayFromStart(); // Timeline 시작
		}

		//UE_LOG(LogTemp, Warning, TEXT("%s"),*TargetLocation.ToString());
		bDidDodge = true;

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			{
				bDidDodge = false;
			}
		), DodgeCooltime, false, -1.f);
	}
}

void UABCharacterMovementComponent::MoveTLFinished()
{
}

void UABCharacterMovementComponent::SetMoveTLUpdate(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), Value);
	FVector cVec = UKismetMathLibrary::VLerp(DodgeStartLocation, DodgeTargetLocation, Value);
	CharacterOwner->SetActorLocation(cVec);
}

FNetworkPredictionData_Client* UABCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UABCharacterMovementComponent* MutableThis = const_cast<UABCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FABNetworkPredictionData_Client_Character(*this);
	}

	return ClientPredictionData;
}

//매 틱마다 실행
//부모 정의는 비어있음.
void UABCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	//텔레포트============================
	if (bPressedTeleport && !bDidTeleport) {
		ABTeleport();
	}
	if (bPressedTeleport) {
		bPressedTeleport = false;
	}
	//텔레포트============================

	//닷지===============================
	if (bPressedDodge && !bDidDodge ) {
		ABDodge();
	}
	if (bPressedDodge) {
		bPressedDodge = false;
	}

	//닷지===============================
}


//클라에서 보낸 데이터를 서버의 캐릭터에서 디코딩.
void UABCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bPressedTeleport = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bDidTeleport = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

	bPressedDodge = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;
	bDidDodge = (Flags & FSavedMove_Character::FLAG_Custom_3) != 0;

	//현재 역할이 서버라면
	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_Authority) {
		if (bPressedTeleport && !bDidTeleport) {
			//AB_SUBLOG(LogABTeleport, Log, TEXT("%s"), TEXT("Teleport Begin"));
			ABTeleport();
		}
		if (bPressedDodge && !bDidDodge) {
			ABDodge();
		}
	}

}

FABNetworkPredictionData_Client_Character::FABNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr FABNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FABSavedMove_Character()); //새로운 움직임 저장 후 반환
}

void FABSavedMove_Character::Clear()
{
	Super::Clear();

	bPressedTeleport = false;
	bDidTeleport = false;

	bPressedDodge = false;
	bDidDodge = false;
}

//텔레포트 하기 전에 무브먼트 컴포넌트에 설정된 속성값을 캐릭터 움직임 데이터에 그대로 저장.
void FABSavedMove_Character::SetInitialPosition(ACharacter* Character)
{
	Super::SetInitialPosition(Character);

	UABCharacterMovementComponent* ABMovement = Cast<UABCharacterMovementComponent>(Character->GetCharacterMovement());
	if (ABMovement) {
		bPressedTeleport = ABMovement->bPressedTeleport;
		bDidTeleport = ABMovement->bDidTeleport;

		bPressedDodge = ABMovement->bPressedDodge;
		bDidDodge = ABMovement->bDidDodge;
	}
}

//압축해서 서버에 보내기.
uint8 FABSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if (bPressedTeleport) {
		Result = FLAG_Custom_0;
	}
	if (bDidTeleport) {
		Result = FLAG_Custom_1;
	}

	if (bPressedDodge) {
		Result = FLAG_Custom_2;
	}
	if (bDidDodge) {
		Result = FLAG_Custom_3;
	}
	return Result;
}
