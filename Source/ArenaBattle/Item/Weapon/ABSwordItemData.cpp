
#include "ABSwordItemData.h"
#include "Character/ABCharacterPlayer.h"
#include "Character/ABCharacterMovementComponent.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "Character/ABComboActionData.h"
#include "GameFramework/GameStateBase.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

UABSwordItemData::UABSwordItemData()
{
	Type = EItemType::Weapon_Sword;
	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(TEXT("/Script/ArenaBattle.ABComboActionData'/Game/ArenaBattle/CharacterAction/ABA_ComboAttack.ABA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		AttackMontage = ComboActionMontageRef.Object;
	}
}

bool UABSwordItemData::CollisionCheck(const UWorld& World, FHitResult& OutHit, FVector& Start, FVector& End)
{
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SwordAttack), false, player);
	const float AttackRange = player->Stat->GetTotalStat().AttackRange;
	const float AttackRadius = player->Stat->GetAttackRadius();
	const float AttackDamage = player->Stat->GetTotalStat().Attack;
	FVector Forward = player->GetActorForwardVector();
	Start = player->GetActorLocation() + Forward * player->GetCapsuleComponent()->GetScaledCapsuleRadius();
	End = Start + player->GetActorForwardVector() * AttackRange;
	return World.SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
}

bool UABSwordItemData::Attack(bool Authority, bool IsLocally)
{
	if (!player) return false;

	if (!Authority) {
		AttackAnim(player->GetMesh()->GetAnimInstance());
	}
	else if(!IsLocally){ //서버일 때 && 자신이 서버가 아닐 경우
		AttackAnim(player->GetMesh()->GetAnimInstance());
	}
	AttackDecreaseSpeed = 0.1f;
	return true;
}

void UABSwordItemData::AttackFinished()
{
}

void UABSwordItemData::AttackAnim(UAnimInstance* AnimInstance)
{
	ProcessComboCommand();
}

FString UABSwordItemData::GetSocketName()
{
	return "hand_rSocket";
}

void UABSwordItemData::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}



void UABSwordItemData::ComboActionBegin()
{
	CurrentCombo = 1;
	//player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	const float AttackSpeedRate = player->Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = player->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UABSwordItemData::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void UABSwordItemData::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);

	CurrentCombo = 0;
	AttackDecreaseSpeed = 1.f;

	//player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void UABSwordItemData::NotifyComboActionEnd()
{
}

void UABSwordItemData::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = player->Stat->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UABSwordItemData::ComboCheck, ComboEffectiveTime, false);
	}
}

void UABSwordItemData::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	AttackDecreaseSpeed = 1.f;

	if (HasNextComboCommand)
	{

		UAnimInstance* AnimInstance = player->GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, AttackMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}


void UABSwordItemData::AttackDrawDebug(const FVector HitLocation, const FVector ActorBoxCenter, const FVector Start, const FVector End)
{
	DrawDebugSphere(GetWorld(), HitLocation, 50.0f, 10, FColor::Green, false, 2.0f);
	//DrawDebugPoint(GetWorld(), HitLocation, 50.0f, FColor::Magenta, false, 5.0f);
}

void UABSwordItemData::AttackDrawDebug(const FVector Start, const FVector End, const FVector Forward)
{
	DrawDebugSphere(GetWorld(), Start, 50.0f, 10, FColor::Red, false, 2.0f);
	//DrawDebugSphere(GetWorld(), HitLocation, 50.0f, 10, FColor::Cyan, false, 2.0f);
	//DrawDebugPoint(GetWorld(), End, 50.0f, FColor::Magenta, false, 5.0f);
}
