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

	bool Attack(bool Authority, bool IsLocally) override;
	void AttackFinished() override;
	void AttackAnim(UAnimInstance* AnimInstance) override;
	FString GetSocketName() override;

	void AttackDrawDebug(const FVector HitLocation, const FVector ActorBoxCenter, const FVector Start, const FVector End) override;
	void AttackDrawDebug(const FVector Start, const FVector End, const FVector Forward) override;

	//combo attack
	void ProcessComboCommand();
	void ComboActionBegin();
	void ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

public:
	UABSwordItemData();

	bool CollisionCheck(const UWorld& World, FHitResult& OutHit, FVector& Start, FVector& End) override;
	
};
