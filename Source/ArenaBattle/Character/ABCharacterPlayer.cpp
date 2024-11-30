// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "ABCharacterControlData.h"
#include "ABCharacterMovementComponent.h"
#include "CharacterStat/ABCharacterStatComponent.h"

#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/ABHUDWidget.h"
#include "Interface/ABGameInterface.h"
#include "ArenaBattle.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ABCollision.h"

#include "EngineUtils.h"
#include "Engine/DamageEvents.h"
#include "Engine/AssetManager.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Components/WidgetComponent.h"
#include "Item/Weapon/ABWeaponItemData.h"
#include "Item/Weapon/ABGunItemData.h"
#include "Item/Weapon/ABSwordItemData.h"
#include "Animation/ABAnimInstance.h"


AABCharacterPlayer::AABCharacterPlayer(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UABCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackReleaseRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Attack_Release.IA_Attack_Release'"));
	if (nullptr != InputActionAttackReleaseRef.Object)
	{
		AttackActionRelease = InputActionAttackReleaseRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTeleportRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Teleport.IA_Teleport'"));
	if (nullptr != InputActionTeleportRef.Object)
	{
		TeleportAction = InputActionTeleportRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRollRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Roll.IA_Roll'"));
	if (nullptr != InputActionRollRef.Object)
	{
		RollAction = InputActionRollRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Quater;
	bCanAttack = true;
}

void AABCharacterPlayer::BeginPlay()
{
	//AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void AABCharacterPlayer::SetDead()
{
	Super::SetDead();

	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &AABCharacterPlayer::ResetPlayer, 5.f, false);

	/*APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);
	}*/
}

void AABCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UpdateMeshFromPlayerState();
}

void AABCharacterPlayer::OnRep_Owner()
{
	//AB_LOG(LogABNetwork, Log, TEXT("%s %s"),*GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	//나를 누가 소유하는지?
	AActor* OwnerActor = GetOwner();
	if (OwnerActor) {
		//AB_LOG(LogABNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else {
		//AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	//AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABCharacterPlayer::PostNetInit()
{
	//AB_LOG(LogABNetwork, Log, TEXT("%s %s"), TEXT("Begin"),*GetName());

	Super::PostNetInit();

	//AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);

	EnhancedInputComponent->BindAction(TeleportAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Teleport);
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Roll);

	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(AttackActionRelease, ETriggerEvent::Triggered, this, &AABCharacterPlayer::GunAttackFinished);
}

void AABCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	if (!IsLocallyControlled())
	{
		return;
	}

	UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	//공격을 할 수 없는 경우에는 움직이지 못하게
	if (!bCanAttack) {
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f) //두개 이상 누르면
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else //한 방향
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	if (CurrentWeapon) MovementVectorSize *= CurrentWeapon->AttackDecreaseSpeed;
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AABCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//서버와 클라이언트간에 해당 변수 동기화.
	//서버가 값을 바꾸면 클라이언트에도 적용. 반대는 적용 안됨.

	DOREPLIFETIME(AABCharacterPlayer, bCanAttack);
	//DOREPLIFETIME(AABCharacterPlayer, CurrentWeapon); //CurrentWeapon이 AActor 가 아니라서 네트워크 복제가 안됨.

}

//무기 장착
void AABCharacterPlayer::EquipWeapon(UABItemData* InItemData)
{
	UABWeaponItemData* WeaponItemData = Cast<UABWeaponItemData>(InItemData);

	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		UABAnimInstance* AnimInstance = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());

		if (WeaponItemData->Type == EItemType::Weapon_Gun) {
			CurrentWeapon = NewObject<UABGunItemData>(this);
			Stat->SetCurrentStat(ECharacterStatus::GunMode);
			AnimInstance->ChangeGunMode(true);
		}
		if (WeaponItemData->Type == EItemType::Weapon_Sword) {
			CurrentWeapon = NewObject<UABSwordItemData>(this);
			Stat->SetCurrentStat(ECharacterStatus::SwordMode);
			AnimInstance->ChangeGunMode(false);
		}

		if (SMWeapon && CurrentWeapon) {
			CurrentWeapon->Init(*this);
			SMWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(CurrentWeapon->GetSocketName()));
			SMWeapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
		}
	}
	if (HasAuthority()) {
		if (WeaponItemData) {
			Stat->SetModifierStat(WeaponItemData->ModifierStat);
		}
	}

}

//공격!!
void AABCharacterPlayer::Attack()
{
	IsAttackClick = true;

	if (!CurrentWeapon) return;
	if (GetWorld()->GetGameState()->GetServerWorldTimeSeconds() - LastAttackStartTime < 0.15f) return;
	LastAttackStartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	
	//[에러] 가끔 공격을 하다보면 캐릭터가 움직이지 않는 현상 발생.
	//-> Attack 뿐만 아니라 Roll 하면 크래쉬남. 근데 점프랑 방향 전환은 됨. 움직이지 않음.
	if (!CurrentWeapon->Attack(false,IsLocallyControlled())) return;

	ServerRPCAttack(LastAttackStartTime);
}

//HasAuthority() 에서 실행할 때만 발동.
//클라에서 ServerRPCAttack을 호출하면 바로 _Implementation으로 이동.
bool AABCharacterPlayer::ServerRPCAttack_Validate(float AttackStartTime)
{
	return true;
}

void AABCharacterPlayer::ServerRPCAttack_Implementation(float AttackStartTime)
{
	CurrentWeapon->Attack(true, IsLocallyControlled());

	//나를 제외한 다른 클라에서도 실행.
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld())) {
		if (!PlayerController || GetController() == PlayerController)  continue;
		if (PlayerController->IsLocalController()) continue;
		AABCharacterPlayer* OtherPlayer = Cast<AABCharacterPlayer>(PlayerController->GetPawn());
		if (!OtherPlayer) continue;

		OtherPlayer->ClientRPCPlayAnimation(this); //다른 클라에있는 나의 애니메이션을 실행
	}
}

//다른 클라에 있는 나.
void AABCharacterPlayer::ClientRPCPlayAnimation_Implementation(AABCharacterPlayer* CharacterToPlay)
{
	if (!CharacterToPlay || !CharacterToPlay->CurrentWeapon) return;
	//CharacterToPlay->CurrentWeapon->AttackAnim(CharacterToPlay->GetMesh()->GetAnimInstance());
	CharacterToPlay->CurrentWeapon->Attack(false, false);
}

void AABCharacterPlayer::GunAttackFinished()
{
	IsAttackClick = false;

	if (bCanAttack || Stat->GetCurrentStat() != ECharacterStatus::GunMode) return;

	float cTime = 0.28f - (GetWorld()->GetGameState()->GetServerWorldTimeSeconds() - CurrentWeapon->AttackStartTime);
	cTime = std::max(KINDA_SMALL_NUMBER, cTime);

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateLambda([&]
		{
			CurrentWeapon->AttackFinished();
			ServerRPCFinishAttack();
		}
	), cTime, false,-1.f);
}

void AABCharacterPlayer::AttackDodge()
{
	UABCharacterMovementComponent* ABMovement = Cast<UABCharacterMovementComponent>(GetCharacterMovement());
	if (ABMovement) {
		ABMovement->SetDodgeCommand();
	}
}

void AABCharacterPlayer::AttackHitCheck()
{
	//소유권을 가진 클라이언트에서 판정 check
	if (!IsLocallyControlled()) return;

	FHitResult OutHitResult;
	bool HitDetected{};
	FVector Start;
	FVector End;

	HitDetected = CurrentWeapon->CollisionCheck(*GetWorld(), OutHitResult, Start, End);

	float HitCheckTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	if (!HasAuthority()) {
		if (HitDetected) {
			ServerRPCNotifyHit(OutHitResult,Start,End, HitCheckTime);
		}
		else {
			ServerRPCNotifyMiss(Start, End, GetActorForwardVector(), HitCheckTime);
		}
	}
	else { //서버가 주인공
		if (HitDetected) {
			CurrentWeapon->AttackDrawDebug(OutHitResult.Location, OutHitResult.Location, Start, End);
			AttackHitConfirm(OutHitResult.GetActor());
		}
		else {
			CurrentWeapon->AttackDrawDebug(Start, End,Start);
		}
	}
}
void AABCharacterPlayer::AttackHitConfirm(AActor* HitActor)
{
	//AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	if (HasAuthority()) {
		//DrawDebugSphere(GetWorld(), HitActor->GetTargetLocation(), 50.f, 12, FColor::Yellow, false, 2.0f);
		const float AttackDamage = Stat->GetTotalStat().Attack;
		FDamageEvent DamageEvent;
		AABCharacterPlayer* HitCharacter = Cast<AABCharacterPlayer>(HitActor);
		if (HitCharacter) {
			HitCharacter->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}
}
void AABCharacterPlayer::DrawDebugAttackRange(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
#if ENABLE_DRAW_DEBUG
	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
#endif
}

//공격 취소
bool AABCharacterPlayer::ServerRPCFinishAttack_Validate()
{
	return true;
}

//공격 취소
void AABCharacterPlayer::ServerRPCFinishAttack_Implementation()
{
	bCanAttack = true;
	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f); 


	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld())) {
		if (!PlayerController || GetController() == PlayerController)  continue;
		if (PlayerController->IsLocalController()) continue;

		AABCharacterPlayer* OtherPlayer = Cast<AABCharacterPlayer>(PlayerController->GetPawn());
		if (!OtherPlayer) continue;

		OtherPlayer->ClientRPCStopAnimation(this);
	}
}




void AABCharacterPlayer::ClientRPCStopAnimation_Implementation(AABCharacterPlayer* CharacterToPlay)
{
	if (!CharacterToPlay) return;

	CharacterToPlay->bCanAttack = true;
	CharacterToPlay->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
}


//시전자 본인에게도 패킷을 보내므로 ClientRPC를 사용하는 것이 최적화에 좋음.
void AABCharacterPlayer::MulticastRPCAttack_Implementation()
{
	if (!IsLocallyControlled()) {
		//PlayAttackAnim();
	}
}

bool AABCharacterPlayer::ServerRPCNotifyHit_Validate(const FHitResult& HitResult, FVector Start, FVector End, float HitCheckTime)
{
	return (HitCheckTime - LastAttackStartTime) > AcceptMinCheckTime;
} 

void AABCharacterPlayer::ServerRPCNotifyHit_Implementation(const FHitResult& HitResult,FVector Start, FVector End, float HitCheckTime)
{
	AActor* HitActor = HitResult.GetActor();
	if (::IsValid(HitActor)) {
		const FVector HitLocation = HitResult.Location;
		const FBox HitBox = HitActor->GetComponentsBoundingBox();
		const FVector ActorBoxCenter = (HitBox.Min + HitBox.Max) * 0.5f;
		if (FVector::DistSquared(HitLocation, ActorBoxCenter) <= AcceptCheckDistance * AcceptCheckDistance) {
			AttackHitConfirm(HitActor);
		}
		else { //거리를 벗어난다면
			AB_LOG(LogABNetwork, Warning, TEXT("%s"), TEXT("HitTest Rejected"));
		}
#if ENABLE_DRAW_DEBUG
		CurrentWeapon->AttackDrawDebug(HitLocation, ActorBoxCenter, Start, End);
#endif
		//DrawDebugAttackRange(FColor::Green, HitResult.TraceStart, HitResult.TraceEnd, HitActor->GetActorForwardVector());
	}
}

bool AABCharacterPlayer::ServerRPCNotifyMiss_Validate(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	return (HitCheckTime-LastAttackStartTime) > AcceptMinCheckTime;
}
void AABCharacterPlayer::ServerRPCNotifyMiss_Implementation(FVector_NetQuantize TraceStart, FVector_NetQuantize TraceEnd, FVector_NetQuantizeNormal TraceDir, float HitCheckTime)
{
	CurrentWeapon->AttackDrawDebug(TraceStart, TraceEnd, TraceDir);
	//DrawDebugAttackRange(FColor::Red, TraceStart, TraceEnd, TraceDir);
}

//클라이언트에서만 자동으로 호출되기 때문에 서버에서는 명시적으로 호출해줘야 함.
void AABCharacterPlayer::OnRep_CanAttack()
{
	if (!bCanAttack) {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AABCharacterPlayer::SetupHUDWidget(UABHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp(), Stat->GetMaxHp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &UABHUDWidget::UpdateHpBar);
	}
}

void AABCharacterPlayer::Teleport()
{
	UABCharacterMovementComponent* ABMovement = Cast<UABCharacterMovementComponent>(GetCharacterMovement());
	if (ABMovement) {
		ABMovement->SetTeleportCommand();
	}
}

void AABCharacterPlayer::Roll()
{
	UABCharacterMovementComponent* ABMovement = Cast<UABCharacterMovementComponent>(GetCharacterMovement());
	if (ABMovement) {
		ABMovement->SetRollCommand();
	}
}

void AABCharacterPlayer::ResetPlayer()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->StopAllMontages(0.f);
	}

	Stat->SetLevelStat(1);
	Stat->ResetStat();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	SetActorEnableCollision(true);
	HpBar->SetHiddenInGame(false);

	if (HasAuthority()) {
		IABGameInterface* ABGameMode = GetWorld()->GetAuthGameMode<IABGameInterface>();
		if (ABGameMode) {
			FTransform NewTransform = ABGameMode->GetRandomStartTransform();
			TeleportTo(NewTransform.GetLocation(), NewTransform.GetRotation().Rotator());
		}
	}
}

void AABCharacterPlayer::ResetAttack()
{
	bCanAttack = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

float AABCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Stat->GetCurrentHp() <= 0.f) {
		IABGameInterface* ABGameMode = GetWorld()->GetAuthGameMode<IABGameInterface>();
		if (ABGameMode) {
			ABGameMode->OnPlayerKilled(EventInstigator, GetController(), this);
		}
	}

	return ActualDamage;
}

void AABCharacterPlayer::UpdateMeshFromPlayerState()
{
	int32 MeshIndex = FMath::Clamp(GetPlayerState()->PlayerId % PlayerMeshes.Num(), 0, PlayerMeshes.Num() - 1);
	MeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(PlayerMeshes[MeshIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterBase::MeshLoadCompleted));
}

void AABCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdateMeshFromPlayerState();
}

