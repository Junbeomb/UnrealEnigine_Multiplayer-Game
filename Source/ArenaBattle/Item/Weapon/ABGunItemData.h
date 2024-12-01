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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletTracer> BulletTracer;

	TObjectPtr<class UParticleSystem> MuzzleParticle;  // TObjectPtr À¯Áö

protected:

	void Attack() override;
	void AttackFinished() override;
	void AttackAnim(UAnimInstance* AnimInstance) override;
	void AttackAnimEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	FString GetSocketName() override;

	void AttackDrawDebug(const FVector HitLocation, const FVector ActorBoxCenter, const FVector Start, const FVector End) override;
	void AttackDrawDebug(const FVector Start, const FVector End, const FVector Forward) override;
public:
	UABGunItemData();

	bool CollisionCheck(const UWorld& World, FHitResult& OutHit, FVector& Start, FVector& End) override;
};
