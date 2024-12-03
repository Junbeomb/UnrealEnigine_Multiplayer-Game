
#include "Item/ABItemSelectBox.h"
#include "Components/SphereComponent.h" 
#include "ABItemData.h"

AABItemSelectBox::AABItemSelectBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerBox"));
	RootComponent = Trigger;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SelectMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Questions/SM_Question.SM_Question'"));
	if (SelectMeshRef.Object) {
		Mesh->SetStaticMesh(SelectMeshRef.Object);
	}

}

void AABItemSelectBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AABItemSelectBox::BeginPlay()
{
	Super::BeginPlay();
	if (Item && Item->SMShowMesh)
		Mesh->SetStaticMesh(Item->SMShowMesh);
	else {


	}
}

// Called every frame
void AABItemSelectBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

