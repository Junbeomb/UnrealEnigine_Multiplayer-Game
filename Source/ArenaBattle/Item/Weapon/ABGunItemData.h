// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/ABWeaponItemData.h"
#include "ABGunItemData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABGunItemData : public UABWeaponItemData
{
	GENERATED_BODY()

protected:

	void Attack(void(AABCharacterPlayer::*AttackHitCheck)(), bool Authority) override;
	void AttackAnim(UAnimInstance* AnimInstance) override;
	FName GetSocketName() override;

public:
	UABGunItemData();
};
