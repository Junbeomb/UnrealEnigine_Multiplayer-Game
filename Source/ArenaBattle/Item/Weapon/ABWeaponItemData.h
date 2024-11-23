
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

	//생성자
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
	//TSoftObjectPtr : 런타임에 객체를 직접 로드하지 않고 경로 정보만 보관한다.
	//에셋이 실제로 필요할 때 로드 한다. -> 초기 로딩 속도 개선
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FABCharacterStat ModifierStat;


};
