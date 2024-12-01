// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGunItemData.h"
#include "Character/ABCharacterPlayer.h"
#include "GameFramework/GameStateBase.h"
#include "DrawDebugHelpers.h"
#include "BulletTracer.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

UABGunItemData::UABGunItemData()
{
	Type = EItemType::Weapon_Gun;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GunFireMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/RIfleAnim/AM_RifleFire.AM_RifleFire'"));
	if (GunFireMontageRef.Object)
	{
		AttackMontage = GunFireMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Item/Weapon/CS_MuzzleFlash.CS_MuzzleFlash'"));
	if (ParticleSystemAsset.Succeeded())
	{
		MuzzleParticle = ParticleSystemAsset.Object;
	}
}

bool UABGunItemData::Attack(bool Authority, bool IsLocally)
{
	if (!player) return false;

	FVector MuzzleLoc = player->GetGunWeapon()->GetSocketLocation("b_gun_muzzleflash");
	FRotator MuzzleRot = player->GetActorForwardVector().Rotation();

	player->bCanAttack = false;
	AttackStartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	AttackAnim(player->GetMesh()->GetAnimInstance());
	GetWorld()->SpawnActor<ABulletTracer>(ABulletTracer::StaticClass(), MuzzleLoc, MuzzleRot);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, MuzzleLoc, MuzzleRot);

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

bool UABGunItemData::CollisionCheck(const UWorld& World, FHitResult& OutHit, FVector& Start, FVector& End)
{
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(GunAttack), false, player);

	FVector Forward = player->GetActorForwardVector(); // 캐릭터의 앞 방향 벡터
	Start = player->GetGunWeapon()->GetSocketLocation("b_gun_muzzleflash"); // 캐릭터의 현재 위치
	End = Start + (Forward * 1000.0f); // 앞 방향으로 1000 단위 거리


	return World.SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.f), TraceParams);
}

void UABGunItemData::AttackDrawDebug(const FVector HitLocation, const FVector ActorBoxCenter, const FVector Start, const FVector End)
{
	DrawDebugSphere(GetWorld(), HitLocation, 50.0f, 10, FColor::Green, false, 2.0f);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.f, 0, 2.5f);
	//DrawDebugSphere(GetWorld(), HitLocation, 50.f, 12, FColor::Green, false, 2.0f);
}

void UABGunItemData::AttackDrawDebug(const FVector Start, const FVector End, const FVector Forward)
{
	DrawDebugSphere(GetWorld(), Start, 50.0f, 10, FColor::Red, false, 2.0f);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0, 2.f);
}

