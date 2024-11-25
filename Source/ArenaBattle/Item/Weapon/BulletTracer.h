// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletTracer.generated.h"

UCLASS()
class ARENABATTLE_API ABulletTracer : public AActor
{
	GENERATED_BODY()

	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	ABulletTracer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
