// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ABSaveGame.h"
#include "ArenaBattle.h"

DEFINE_LOG_CATEGORY(LogABPlayerController);

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C"));
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

//네트웍과 무관하게 액터를 초기화할 때 사용
void AABPlayerController::PostInitializeComponents()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

//원격 클라에서 네트워크로 초기화에 필요한 정보를 받은 것이 모두 마무리가 되면 호출됨.
void AABPlayerController::PostNetInit()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();
	UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver->ServerConnection) {
		AB_LOG(LogABNetwork, Log, TEXT("Server Connections : %s"), *NetDriver->ServerConnection->GetName());
	}
	else {
		AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("NO NetDriver"));
	}

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AABPlayerController::BeginPlay()
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::BeginPlay();

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}

void AABPlayerController::OnPossess(APawn* InPawn)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}
