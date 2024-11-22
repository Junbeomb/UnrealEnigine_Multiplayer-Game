// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ARENABATTLE_API WeaponTraceCheck
{
public:

	WeaponTraceCheck();
	~WeaponTraceCheck();

	bool SwordTraceCheck(const UWorld& World, class AABCharacterPlayer&, FHitResult& OutHit);
	bool GunTraceCheck(const UWorld& World, class AABCharacterPlayer&, FHitResult& OutHit);
};
