// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGunItemData.h"
#include "Character/ABCharacterPlayer.h"
#include "GameFramework/GameStateBase.h"

UABGunItemData::UABGunItemData()
{
	Type = EItemType::Weapon_Gun;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GunFireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/RIfleAnim/AM_RifleFire.AM_RifleFire'"));
	if (GunFireMontageRef.Object)
	{
		AttackMontage = GunFireMontageRef.Object;
	}
}



bool UABGunItemData::Attack(bool Authority, bool IsLocally)
{
	if (!player) return false;

	if (!Authority) {
		player->bCanAttack = false;
		AttackStartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		AttackAnim(player->GetMesh()->GetAnimInstance());
	}
	else if (!IsLocally){
		AttackAnim(player->GetMesh()->GetAnimInstance());
	}

	return true;
}

void UABGunItemData::AttackFinished()
{
	player->bCanAttack = true;
	player->GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);
}

void UABGunItemData::AttackAnim(UAnimInstance* AnimInstance)
{
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackMontage);
}

FString UABGunItemData::GetSocketName()
{
	return "hand_rSocket_Gun";
}
