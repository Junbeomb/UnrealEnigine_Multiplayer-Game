
#include "ABWeaponItemData.h"
#include "Character/ABCharacterBase.h"
#include "Character/ABCharacterPlayer.h"

void UABWeaponItemData::Init(AABCharacterBase& p)
{
	player = Cast<AABCharacterPlayer>(&p);
	AttackWalkDecrease = 1.f;
}
