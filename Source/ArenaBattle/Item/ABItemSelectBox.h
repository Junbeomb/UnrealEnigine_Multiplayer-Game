// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemSelectBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemSelectBox : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Select)
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(EditAnywhere, Category = Select)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = Select)
	TObjectPtr<class UABItemData> Item;

	UPROPERTY(EditAnywhere, Category = Select)
	TObjectPtr<class UParticleSystemComponent> Effect;


	
public:	
	AABItemSelectBox();
	FORCEINLINE class USphereComponent* GetTrigger() { return Trigger; }

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	//UFUNCTION()
	//void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	//UFUNCTION()
	//void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);

public:	
	virtual void Tick(float DeltaTime) override;

};
