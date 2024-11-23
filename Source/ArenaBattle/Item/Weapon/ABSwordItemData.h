// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/ABWeaponItemData.h"
#include "ABSwordItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABSwordItemData : public UABWeaponItemData
{
	GENERATED_BODY()

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;

protected:

	void Attack(void(AABCharacterPlayer::*AttackHitCheck)(), bool Authority) override;
	void AttackAnim(UAnimInstance* AnimInstance) override;
	FName GetSocketName() override;

	//combo attack
	void ProcessComboCommand();
	void ComboActionBegin();
	void ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

public:
	UABSwordItemData();
	
};
