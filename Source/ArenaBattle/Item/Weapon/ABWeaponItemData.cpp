
#include "ABWeaponItemData.h"
#include "Character/ABCharacterBase.h"
#include "Character/ABCharacterPlayer.h"

void UABWeaponItemData::Init(AABCharacterBase& p)
{
	bCanAttack = true;
	player = Cast<AABCharacterPlayer>(&p);
}
