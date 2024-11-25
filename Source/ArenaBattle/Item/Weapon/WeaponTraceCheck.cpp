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

bool WeaponTraceCheck::SwordTraceCheck(const UWorld& World, AABCharacterPlayer& player, FHitResult& OutHit, FVector& Start, FVector& End)
{
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SwordAttack), false, &player);
	const float AttackRange = player.Stat->GetTotalStat().AttackRange;
	const float AttackRadius = player.Stat->GetAttackRadius();
	const float AttackDamage = player.Stat->GetTotalStat().Attack;
	FVector Forward = player.GetActorForwardVector();
	Start = player.GetActorLocation() + Forward * player.GetCapsuleComponent()->GetScaledCapsuleRadius();
	End = Start + player.GetActorForwardVector() * AttackRange;
	return World.SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
}

bool WeaponTraceCheck::GunTraceCheck(const UWorld& World, AABCharacterPlayer& player, FHitResult& OutHit, FVector& Start, FVector& End)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(GunAttack), false, &player);

	FVector Forward = player.GetActorForwardVector(); // ĳ������ �� ���� ����
	Start = player.GetGunWeapon()->GetSocketLocation("b_gun_muzzleflash"); // ĳ������ ���� ��ġ
	End = Start + (Forward * 1000.0f); // �� �������� 1000 ���� �Ÿ�


	return World.SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.f), TraceParams);
}


