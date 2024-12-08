
#include "ABSelectComp.h"
#include "Components/SphereComponent.h"
#include "Character/ABCharacterPlayer.h"
#include "ABItemSelectBox.h"

UABSelectComp::UABSelectComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> OverlayMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/ArenaBattle/Item/MI_InteractOverlay.MI_InteractOverlay'"));
	if (OverlayMaterialRef.Succeeded())
	{
		OverlayMat = OverlayMaterialRef.Object;
	}

}


void UABSelectComp::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()) {
		UActorComponent* ActorComp = GetOwner()->AddComponentByClass(USphereComponent::StaticClass(), false, FTransform::Identity, false);
		SphereComp = Cast<USphereComponent>(ActorComp);
		SphereComp->SetHiddenInGame(false);
		SphereComp->SetSphereRadius(60.f);

		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &UABSelectComp::OnOverlapBegin);
		SphereComp->OnComponentEndOverlap.AddDynamic(this, &UABSelectComp::OnOverlapEnd);

		OwnerMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	}

}

void UABSelectComp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AABCharacterPlayer* player = Cast<AABCharacterPlayer>(OtherActor);
	if (player) {
		player->ItemOverlapToggle = true;
		player->CurrentSelectBox = Cast<AABItemSelectBox>(GetOwner());
		IsOverlap = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
		if (OverlayMat) {
			OwnerMesh->SetOverlayMaterial(OverlayMat);
		}

	}
}

void UABSelectComp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AABCharacterPlayer* player = Cast<AABCharacterPlayer>(OtherActor);
	if (player) {
		player->ItemOverlapToggle = false;
		player->CurrentSelectBox = nullptr;
		IsOverlap = false;
		UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
		OwnerMesh->SetOverlayMaterial(nullptr);
	}
}

void UABSelectComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

