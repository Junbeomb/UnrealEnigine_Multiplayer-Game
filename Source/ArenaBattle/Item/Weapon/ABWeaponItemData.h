
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

	AABCharacterBase* player;
public:

	//������
	UABWeaponItemData() = default;
	void Init(AABCharacterBase* p)
	{
		bCanAttack = true;
		player = p;
	}

	UPROPERTY()
	uint8 bCanAttack;

	virtual void Attack(void(AABCharacterPlayer::* AttackHitCheck)(), bool Authority) {};
	virtual void AttackAnim(UAnimInstance* AnimInstance) {};
	virtual FName GetSocketName() { return FName(FString("")); };

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
