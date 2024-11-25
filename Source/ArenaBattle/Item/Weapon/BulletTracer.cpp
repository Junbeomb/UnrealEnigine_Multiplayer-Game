// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/BulletTracer.h"

// Sets default values
ABulletTracer::ABulletTracer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABulletTracer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletTracer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

