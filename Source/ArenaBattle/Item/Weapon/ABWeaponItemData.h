
#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "GameData/ABCharacterStat.h"
#include "ABWeaponItemData.generated.h"

/**
 * 
 */
class AABCharacterBase;
class AABCharacterPlayer;
UCLASS()
class ARENABATTLE_API UABWeaponItemData : public UABItemData
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	TObjectPtr<AABCharacterPlayer> player;
public:

	//������
	UABWeaponItemData() = default;
	void Init(AABCharacterBase& p);

	float AttackStartTime;

	float AttackWalkDecrease;

	virtual bool Attack(bool Authority, bool IsLocally) { return true; };
	virtual void AttackFinished() {};
	virtual void AttackAnim(UAnimInstance* AnimInstance) {};
	virtual FString GetSocketName() { return ""; };

	virtual void AttackDrawDebug(const FVector HitLocation, const FVector ActorBoxCenter, const FVector Start, const FVector End) {};
	virtual void AttackDrawDebug(const FVector Start, const FVector End, const FVector Forward) {};

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
	}

public:
	//TSoftObjectPtr : ��Ÿ�ӿ� ��ü�� ���� �ε����� �ʰ� ��� ������ �����Ѵ�.
	//������ ������ �ʿ��� �� �ε� �Ѵ�. -> �ʱ� �ε� �ӵ� ����
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat ModifierStat;


};
