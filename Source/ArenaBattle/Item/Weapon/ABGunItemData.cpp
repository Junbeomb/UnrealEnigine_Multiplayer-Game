// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGunItemData.h"
#include "Character/ABCharacterPlayer.h"

UABGunItemData::UABGunItemData()
{
	Type = EItemType::Weapon_Gun;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GunFireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/RIfleAnim/AM_RifleFire.AM_RifleFire'"));
	if (GunFireMontageRef.Object)
	{
		AttackMontage = GunFireMontageRef.Object;
	}
}



void UABGunItemData::Attack(void(AABCharacterPlayer::*AttackHitCheck)(), bool Authority)
{
	if (!bCanAttack) return;

	if (!Authority) {
		bCanAttack = false;
	}

	//총은 Notify가 없으므로
	if(AttackHitCheck) (Cast<AABCharacterPlayer>(player)->*AttackHitCheck)();
}

void UABGunItemData::AttackAnim(UAnimInstance* AnimInstance)
{
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackMontage);
}

FName UABGunItemData::GetSocketName()
{
	return FName(FString("hand_rSocket_Gun"));
}
