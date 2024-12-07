
#include "ABSelectComp.h"
#include "Components/SphereComponent.h"
#include "Character/ABCharacterPlayer.h"

UABSelectComp::UABSelectComp()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UABSelectComp::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()) {
		UActorComponent* ActorComp = GetOwner()->AddComponentByClass(USphereComponent::StaticClass(), false, FTransform::Identity, false);
		SphereComp = Cast<USphereComponent>(ActorComp);
		SphereComp->SetHiddenInGame(false);
		SphereComp->SetSphereRadius(200.f);

		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &UABSelectComp::OnOverlapBegin);
		SphereComp->OnComponentEndOverlap.AddDynamic(this, &UABSelectComp::OnOverlapEnd);
	}

}

void UABSelectComp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	AABCharacterPlayer* player = Cast<AABCharacterPlayer>(OtherActor);
	if (player) {
		player->ItemOverlapToggle = true;
	}
}

void UABSelectComp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AABCharacterPlayer* player = Cast<AABCharacterPlayer>(OtherActor);
	if (player) {
		player->ItemOverlapToggle = false;
	}
}

void UABSelectComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

