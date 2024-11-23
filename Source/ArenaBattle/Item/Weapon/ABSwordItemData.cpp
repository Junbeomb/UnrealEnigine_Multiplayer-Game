
#include "ABSwordItemData.h"
#include "Character/ABCharacterPlayer.h"
#include "Character/ABCharacterMovementComponent.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "Character/ABComboActionData.h"

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

bool UABSwordItemData::Attack(bool Authority)
{
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
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
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
	player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

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
