
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

	UPROPERTY()
	uint8 bCanAttack;

	float AttackStartTime;

	virtual bool Attack(bool Authority) { return true; };
	virtual void AttackFinished() {};
	virtual void AttackAnim(UAnimInstance* AnimInstance) {};
	virtual FString GetSocketName() { return ""; };

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
