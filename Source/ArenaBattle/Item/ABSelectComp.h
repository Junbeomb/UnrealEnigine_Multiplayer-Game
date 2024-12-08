
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABSelectComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABSelectComp : public UActorComponent
{
	GENERATED_BODY()



	TObjectPtr<UMaterialInterface> OverlayMat;

	TObjectPtr<UStaticMeshComponent> OwnerMesh;

	bool IsOverlap{ false };

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

public:	
	UABSelectComp();

	TObjectPtr<class USphereComponent> SphereComp;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
