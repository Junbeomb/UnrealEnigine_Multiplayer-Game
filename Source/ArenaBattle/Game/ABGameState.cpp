// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameState.h"
#include "ArenaBattle.h"


void AABGameState::HandleBeginPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::HandleBeginPlay();
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

//HandleState�� ������ ��� ���͵鿡�� BeginPlay() �����ϰ� ��.
void AABGameState::OnRep_ReplicatedHasBegunPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::OnRep_ReplicatedHasBegunPlay();
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}