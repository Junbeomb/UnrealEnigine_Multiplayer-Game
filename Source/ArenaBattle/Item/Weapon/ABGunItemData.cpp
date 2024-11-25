// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGunItemData.h"
#include "Character/ABCharacterPlayer.h"
#include "GameFramework/GameStateBase.h"
#include "DrawDebugHelpers.h"

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

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UABGunItemData::AttackAnimEnd);
	player->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate,AttackMontage);

}

void UABGunItemData::AttackAnimEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (player->IsAttackClick) {
		player->bCanAttack = true;
		player->Attack();
	}
}

FString UABGunItemData::GetSocketName()
{
	return "hand_rSocket_Gun";
}

void UABGunItemData::AttackDrawDebug(const FVector HitLocation, const FVector ActorBoxCenter, const FVector Start, const FVector End)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.f, 0, 2.5f);
	//DrawDebugSphere(GetWorld(), HitLocation, 50.f, 12, FColor::Green, false, 2.0f);
}

void UABGunItemData::AttackDrawDebug(const FVector Start, const FVector End, const FVector Forward)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0, 2.f);
}
