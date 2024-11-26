
#include "Item/Weapon/BulletTracer.h"

ABulletTracer::ABulletTracer()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Item/Weapon/SM_BulletTracer.SM_BulletTracer'"));
	if (MeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(MeshAsset.Object);
	}

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FRotator Rotation = FRotator(90.0f, 0.0f, 0.0f);  // X축 (Pitch) 방향으로 90도 회전
	StaticMesh->SetWorldRotation(Rotation);
	StaticMesh->SetRelativeScale3D({ 2.f,2.f,-10.f });
}

void ABulletTracer::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle BulletTHandle;
	GetWorld()->GetTimerManager().SetTimer(BulletTHandle, FTimerDelegate::CreateLambda([&]
		{
			Destroy();
		}
	), 0.3f, false, -1.f);
	
}

void ABulletTracer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

