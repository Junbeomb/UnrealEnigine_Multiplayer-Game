// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABFountain.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Net/UnrealNetwork.h"
#include "ArenaBattle.h"
// Sets default values
AABFountain::AABFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));

	RootComponent = Body;
	Water->SetupAttachment(Body);
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 132.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));
	if (WaterMeshRef.Object)
	{
		Water->SetStaticMesh(WaterMeshRef.Object);
	}

	bReplicates = true;
	//������Ʈ �ֱ� (�⺻�� : 100)
	NetUpdateFrequency = 1.0f;
	//20���� �ݰ� ���ʿ� �ִ� ���͵鸸 ������ Ȱ��ȭ.
	//������ ����� �ش� Ŭ�� ���� ���ø����̼� ��Ȱ��ȭ.
	NetCullDistanceSquared = 4'000'000.0f; 
	//�޸� ���� ����
	NetDormancy = DORM_Initial;
}

// Called when the game starts or when spawned
void AABFountain::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			{
				//BigData.Init(BigDataElement, 100);
				//BigDataElement += 1.0f;
				//ServerLightColor = FLinearColor(FMath::RandRange(0.0f, 1.f), FMath::RandRange(0.0f, 1.f), FMath::RandRange(0.0f, 1.f), 1.0f);
				//OnRep_ServerLightColor(); //OnRep�Լ��� ������ȣ�� ���� �ʱ� ������ �̷��� ��������� ȣ��.(Ŭ��� �翬�� ȣ��)

				const FLinearColor NewLightColor = FLinearColor(FMath::RandRange(0.0f, 1.f), FMath::RandRange(0.0f, 1.f), FMath::RandRange(0.0f, 1.f), 1.0f);
				MulticastRPCChangeLightColor(NewLightColor);
			}
		), 1.0f, true, 0.0f);

		FTimerHandle Handle2;
		GetWorld()->GetTimerManager().SetTimer(Handle2, FTimerDelegate::CreateLambda([&]
			{
				FlushNetDormancy();
			}
		), 6.0f, false, -1.0f);
	}
	
	
}

// Called every frame
void AABFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		AddActorLocalRotation(FRotator(0.0f, RotationRate * DeltaTime, 0.0f));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
	}
	else {

		//Client�� Server�� Yaw�� ����.
		//Client������ NetUpdateFrequency ���� 1�̹Ƿ�. (Server�� 100)
		ClientTimeSinceUpdate += DeltaTime;
		if (ClientTimeBetWeenLastUpdate < KINDA_SMALL_NUMBER) return;

		const float EstimateRotationYaw = ServerRotationYaw + RotationRate * ClientTimeBetWeenLastUpdate;
		const float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetWeenLastUpdate;

		FRotator ClientRotator = RootComponent->GetComponentRotation();
		const float ClientNewYaw = FMath::Lerp(ServerRotationYaw, EstimateRotationYaw, LerpRatio); //���� Ŭ���̾�Ʈ Tick���� ������ Yaw��.
		ClientRotator.Yaw = ClientNewYaw;
		RootComponent->SetWorldRotation(ClientRotator);
	}


}

void AABFountain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AABFountain, ServerRotationYaw);
	DOREPLIFETIME(AABFountain, ServerLightColor);
	//DOREPLIFETIME_CONDITION(AABFountain, ServerLightColor, COND_InitialOnly);
	//DOREPLIFETIME(AABFountain, BigData);

}

//connection�� ���ؼ� bunch������ �ؼ��ؼ�
//���� ���ø����̼ǿ��� � �۾��� �����ؾ� �ϴ���.
//�������� ��Ż�� ���ȴ�.
void AABFountain::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnActorChannelOpen(InBunch, Connection);

	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("End"));
}

bool AABFountain::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool NetRelevantResult = Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);
	if (!NetRelevantResult) {
		AB_LOG(LogABNetwork, Log, TEXT("Not Relevant : [%s] %s"), *RealViewer->GetName(), *SrcLocation.ToCompactString());
	}
	return NetRelevantResult;
}

//�м��� ���ʹ� ��Ʈ��ũ�� ������ �غ� �Ǿ��ִ�!
void AABFountain::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	AB_LOG(LogABNetwork, Log, TEXT("%s"), TEXT("Begin"));
	Super::PreReplication(ChangedPropertyTracker);
}

void AABFountain::OnRep_ServerRotationYaw()
{
	if (HasAuthority()) {

		//AB_LOG(LogABNetwork, Log, TEXT("Yaw : %f"), ServerRotationYaw);
	}

	//Tick���� ������� �ʰ� �ݹ��Լ����� ����. -> ���� �ݹ��� Tick���� �� ���� ����ϴ� ��쿡 ȿ������.
	FRotator NewRotator = RootComponent->GetComponentRotation();
	NewRotator.Yaw = ServerRotationYaw;
	RootComponent->SetWorldRotation(NewRotator);

	ClientTimeBetWeenLastUpdate = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0.0f;
}

void AABFountain::OnRep_ServerLightColor()
{
	if (!HasAuthority()) {
		//AB_LOG(LogABNetwork, Log, TEXT("LightColor : %s"), *ServerLightColor.ToString());
	}

	UPointLightComponent* PointLight = Cast<UPointLightComponent>(GetComponentByClass(UPointLightComponent::StaticClass()));

	if (PointLight) {
		PointLight->SetLightColor(ServerLightColor);
	}

}

void AABFountain::MulticastRPCChangeLightColor_Implementation(const FLinearColor& NewLightColor)
{
	AB_LOG(LogABNetwork, Log, TEXT("LightColor : %s"), *NewLightColor.ToString());

	UPointLightComponent* PointLight = Cast<UPointLightComponent>(GetComponentByClass(UPointLightComponent::StaticClass()));

	if (PointLight) {
		PointLight->SetLightColor(NewLightColor);
	}

}

