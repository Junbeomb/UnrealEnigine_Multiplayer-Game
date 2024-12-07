
#include "Item/ABItemSelectBox.h"
#include "Components/SphereComponent.h" 
#include "ABItemData.h"
#include "ABSelectComp.h"

AABItemSelectBox::AABItemSelectBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SelectMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Questions/SM_Question.SM_Question'"));
	if (SelectMeshRef.Object) {
		Mesh->SetStaticMesh(SelectMeshRef.Object);
	}

	SelectComp = CreateDefaultSubobject<UABSelectComp>(TEXT("SelectComp"));
	
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
}

void AABItemSelectBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

