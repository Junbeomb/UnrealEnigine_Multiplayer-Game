// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTraceCheck.h"
#include "Character/ABCharacterPlayer.h"
#include "CharacterStat/ABCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"

WeaponTraceCheck::WeaponTraceCheck()
{
}

WeaponTraceCheck::~WeaponTraceCheck()
{
}

bool WeaponTraceCheck::SwordTraceCheck(const UWorld& World, AABCharacterPlayer& player, FHitResult& OutHit)
{
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SwordAttack), false, &player);
	const float AttackRange = player.Stat->GetTotalStat().AttackRange;
	const float AttackRadius = player.Stat->GetAttackRadius();
	const float AttackDamage = player.Stat->GetTotalStat().Attack;
	FVector Forward = player.GetActorForwardVector();
	FVector Start = player.GetActorLocation() + Forward * player.GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector End = Start + player.GetActorForwardVector() * AttackRange;
	return World.SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
}

bool WeaponTraceCheck::GunTraceCheck(const UWorld& World, AABCharacterPlayer& player, FHitResult& OutHit)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(GunAttack), false, &player);

	FVector Forward = player.GetActorForwardVector(); // 캐릭터의 앞 방향 벡터
	FVector Start = player.GetGunWeapon()->GetSocketLocation("b_gun_muzzleflash"); // 캐릭터의 현재 위치
	FVector End = Start + (Forward * 1000.0f); // 앞 방향으로 1000 단위 거리

	DrawDebugLine(&World, Start, End, FColor::Red, false, 2.0f, 0, 3.0f);
	DrawDebugSphere(&World, OutHit.ImpactPoint, 50.f, 12, FColor::Red, false, 2.0f);

	return World.SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.f), TraceParams);
}


