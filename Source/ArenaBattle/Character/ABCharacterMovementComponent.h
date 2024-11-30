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
	virtual uint8 GetCompressedFlags() const override; //Ŭ���̾�Ʈ�� ���������� �÷��� ������ �ٲ�ġ��, �׸��� ������ ������

	uint8 bPressedTeleport : 1;
	uint8 bDidTeleport : 1;

	uint8 bPressedDodge : 1;
	uint8 bDidDodge : 1;

	uint8 bPressedRoll : 1;
	uint8 bDidRoll : 1;
};

//Ŭ���̾�Ʈ �����͸� �����ϴ� Ŭ����
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

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override; //�� ƽ���� ����.
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

//�ڷ���Ʈ==============================================
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
//�ڷ���Ʈ==============================================

//����=================================================
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
//����=================================================

//������=================================================
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
//������=================================================

};
