// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "ABCharacterMovementComponent.generated.h"

class FABSavedMove_Character : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;
public:
	virtual void Clear() override;
	virtual void SetInitialPosition(ACharacter* Character) override;
	virtual uint8 GetCompressedFlags() const override; //클라이언트의 상태정보를 플래그 정보로 바꿔치기, 그리고 서버로 보내기

	uint8 bPressedTeleport : 1;
	uint8 bDidTeleport : 1;

	uint8 bPressedDodge : 1;
	uint8 bDidDodge : 1;

	uint8 bPressedRoll : 1;
	uint8 bDidRoll : 1;
};

//클라이언트 데이터를 관리하는 클래스
class FABNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character 
{
	typedef FNetworkPredictionData_Client_Character Super;
public:
	FABNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement);
	virtual FSavedMovePtr AllocateNewMove() override;
};
/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UABCharacterMovementComponent();

protected:
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override; //매 틱마다 실행.
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

//텔레포트==============================================
public:
	void SetTeleportCommand();
	uint8 bPressedTeleport : 1;
	uint8 bDidTeleport : 1;

protected:
	virtual void ABTeleport();
	UPROPERTY()
	float TeleportOffset;
	UPROPERTY()
	float TeleportCooltime;
//텔레포트==============================================

//닷지=================================================
public:
	void SetDodgeCommand();
	uint8 bPressedDodge : 1;
	uint8 bDidDodge : 1;

	FVector DodgeStartLocation;
	FVector DodgeTargetLocation;

protected:
	virtual void ABDodge();
	UPROPERTY()
	float DodgeOffset;
	UPROPERTY()
	float DodgeCooltime;

	//Timeline
	TObjectPtr<UTimelineComponent> MoveTL;
	FOnTimelineEvent MoveTLFinishedCallback;
	UFUNCTION()
	void MoveTLFinished();
	UPROPERTY(EditAnywhere)
	UCurveFloat* DodgeCurve;
	FOnTimelineFloat  MoveTLCallback;
	UFUNCTION()
	void SetMoveTLUpdate(float Value);
//닷지=================================================

//구르기=================================================
public:
	void SetRollCommand();
	uint8 bPressedRoll : 1;
	uint8 bDidRoll : 1;

	FVector RollStartLocation;
	FVector RollTargetLocation;

protected:
	virtual void ABRoll();
	UPROPERTY()
	float RollOffset;
	UPROPERTY()
	float RollCooltime;

	TObjectPtr<class UAnimMontage> RollMontage;
//구르기=================================================

};
