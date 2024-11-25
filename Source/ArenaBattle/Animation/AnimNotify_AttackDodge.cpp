// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackDodge.h"
#include "Interface/ABAnimationAttackInterface.h"


void UAnimNotify_AttackDodge::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackDodge();
		}
	}
}