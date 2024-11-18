// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameState.h"
#include "ArenaBattle.h"
#include "Net/UnrealNetwork.h"

AABGameState::AABGameState()
{
	RemainingTime = MatchPlayTime;
}

void AABGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABGameState, RemainingTime);
}

//void AABGameState::HandleBeginPlay()
//{
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
//	Super::HandleBeginPlay();
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
//}
//
////HandleState가 월드의 모든 액터들에게 BeginPlay() 시작하게 함.
//void AABGameState::OnRep_ReplicatedHasBegunPlay()
//{
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
//	Super::OnRep_ReplicatedHasBegunPlay();
//	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
//}
