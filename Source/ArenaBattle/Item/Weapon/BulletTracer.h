#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletTracer.generated.h"

UCLASS()
class ARENABATTLE_API ABulletTracer : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
public:	
	ABulletTracer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
