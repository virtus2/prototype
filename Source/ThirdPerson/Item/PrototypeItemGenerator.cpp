// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItemGenerator.h"

#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"

#include "ThirdPerson/PrototypeGameplayTags.h"
#include "ThirdPerson/Data/TreasureClass.h"
#include "ThirdPerson/Data/ItemAffix.h"
#include "ThirdPerson/Data/ItemType.h"
#include "ThirdPerson/Data/ItemBase.h"
#include "ThirdPerson/Data/Weapon.h"
#include "ThirdPerson/Data/Armor.h"
#include "ThirdPerson/Item/PrototypeItem.h"
#include "ThirdPerson/Item/PrototypeItemUtil.h"
#include "ThirdPerson/Game/PrototypeGameModeBase.h"
#include "ThirdPerson/Character/PrototypeHeroCharacter.h"
#include "ThirdPerson/Player/PrototypePlayerState.h"
#include "ThirdPerson/Ability/PrototypeAbilitySystemComponent.h"

UPrototypeItemGenerator::UPrototypeItemGenerator()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> TreasureClassDataTableClass(TEXT("/Game/Prototype/Data/DT_TreasureClass.DT_TreasureClass"));
	if (IsValid(TreasureClassDataTableClass.Object))
	{
		TreasureClassDataTable = TreasureClassDataTableClass.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTypeDataTableClass(TEXT("/Game/Prototype/Data/DT_ItemType.DT_ItemType"));
	if (IsValid(ItemTypeDataTableClass.Object))
	{
		ItemTypeDataTable = ItemTypeDataTableClass.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ArmorDataTableClass(TEXT("/Game/Prototype/Data/DT_Armor.DT_Armor"));
	if (IsValid(ArmorDataTableClass.Object))
	{
		ArmorDataTable = ArmorDataTableClass.Object;

		const FString Context;
		ArmorDataTable->GetAllRows(Context, AllArmorData);
		for (auto& ArmorData : AllArmorData)
		{
			FGameplayTag ItemType = ArmorData->BaseData.ItemType;
			if (ItemTypeToArmorData.Contains(ItemType))
			{
				ItemTypeToArmorData[ItemType].Add(ArmorData);
			}
			else
			{
				TArray<FArmor*> ArmorArray;
				ItemTypeToArmorData.Add(ItemType, ArmorArray);
				ItemTypeToArmorData[ItemType].Add(ArmorData);
			}
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDataTableClass(TEXT("/Game/Prototype/Data/DT_Weapon.DT_Weapon"));
	if (IsValid(WeaponDataTableClass.Object))
	{
		WeaponDataTable = WeaponDataTableClass.Object;

		const FString Context;
		WeaponDataTable->GetAllRows(Context, AllWeaponData);
		for (auto& WeaponData : AllWeaponData)
		{
			FGameplayTag ItemType = WeaponData->BaseData.ItemType;
			if (ItemTypeToWeaponData.Contains(ItemType))
			{
				ItemTypeToWeaponData[ItemType].Add(WeaponData);
			}
			else
			{
				TArray<FWeapon*> WeaponArray;
				ItemTypeToWeaponData.Add(ItemType, WeaponArray);
				ItemTypeToWeaponData[ItemType].Add(WeaponData);
			}
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemAffixDataTableClass(TEXT("/Game/Prototype/Data/DT_ItemAffix.DT_ItemAffix"));
	if (IsValid(ItemAffixDataTableClass.Object))
	{
		ItemAffixDataTable = ItemAffixDataTableClass.Object;
		if (IsValid(ItemAffixDataTable))
		{
			const FString Context;
			ItemAffixDataTable->GetAllRows<FItemAffix>(Context, AllItemAffixes);
			// AffixType에 따라 TMap에 Affix를 넣어둔다.
			for (auto& Affix : AllItemAffixes)
			{
				FGameplayTag AffixType = Affix->AffixType;
				if (AffixTypeToAffixData.Contains(AffixType))
				{
					AffixTypeToAffixData[AffixType].Add(Affix);
				}
				else
				{
					TArray<FItemAffix*> AffixArray;
					AffixTypeToAffixData.Add(AffixType, AffixArray);
					AffixTypeToAffixData[AffixType].Add(Affix);
				}
			}
		}
	}

	static ConstructorHelpers::FObjectFinder<UAttributeGameplayEffectMap> AttributeMap(TEXT("/Game/Prototype/Data/DA_AttributeGameplayEffectMap.DA_AttributeGameplayEffectMap"));
	if (IsValid(AttributeMap.Object))
	{
		ItemAffixAttributes = AttributeMap.Object;
	}
}

/// <summary>
/// TreasureClass에 따라 아이템을 랜덤으로 생성한다. 아이템 생성의 가장 최상위 함수.
/// </summary>
/// <param name="TreasureClass"></param>
/// <param name="MonsterLevel"></param>
/// <returns></returns>
void UPrototypeItemGenerator::GenerateItems(FGameplayTag TreasureClass, int32 MonsterLevel)
{
	const FString Context;
	FTreasureClass* TC = TreasureClassDataTable->FindRow<FTreasureClass>(TreasureClass.GetTagName(), Context);
	if (TC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable(TreasureClass) FindRow(%s) is failed"), *TreasureClass.ToString());
		return;
	}

	// 1. TC에서 ItemType을 뽑는다.
	TArray<FGameplayTag> TCPickedItemTypes = RollTreasureClassPicks(TC, MonsterLevel);
	
	// 위에서 뽑은 ItemType에 대해서, 하위 ItemType을 랜덤하게 뽑는다.
	// 예를 들면 Item.Type.Equipment의 하위 태그 중 하나를 뽑는다.
	TArray<FGameplayTag> PickedItemTypes;
	for (const auto& ItemType : TCPickedItemTypes)
	{
		FGameplayTag PickedItemType = PickItemType(ItemType, MonsterLevel);
		PickedItemTypes.Add(PickedItemType);
	}

	// 2. 최종적으로 뽑힌 아이템타입들에 대해서, 아이템을 생성한다.
	TArray<TObjectPtr<UPrototypeItem>> GeneratedItems;
	for (const auto& ItemType : PickedItemTypes)
	{
		FItemType* ItemTypeData = ItemTypeDataTable->FindRow<FItemType>(ItemType.GetTagName(), Context);
		if (ItemTypeData == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ItemType데이터 테이블에 (%s)에 해당하는 데이터가 없습니다."), *ItemType.ToString());
			return;
		}

		TObjectPtr<UPrototypeItem> Item;
		if (ItemType.MatchesTag(TAG_Item_Type_Gold))
		{
			Item = GenerateItem_Gold(MonsterLevel);
		}
		if (ItemType.MatchesTag(TAG_Item_Type_Equipment))
		{
			Item = GenerateItem_Equipment(ItemType, MonsterLevel, TC, ItemTypeData);
		}

		GeneratedItems.Add(Item);
	}

	// 3. 생성한 아이템을 반환한다.
	// TODO: 반환을 하던지 매개변수로 참조 받던지 해서 결과를 함수 밖으로 내보낼수있어야됨
	for (const auto& Item : GeneratedItems)
	{
		Item->DebugLog();
	}
}

/// <summary>
/// TC에 따라 드롭할 아이템 타입들을 뽑는다.
/// TC가 ItemType일 경우 해당 타입을 뽑고, TC일 경우 재귀적으로 호출해서 뽑을 아이템 타입을 찾는다.
/// https://www.inven.co.kr/board/diablo2/5734/6547의 [첫째, 드랍시킬 아이템의 종류 결정] 참고.
/// </summary>
TArray<FGameplayTag> UPrototypeItemGenerator::RollTreasureClassPicks(FTreasureClass* TreasureClass, int32 MonsterLevel)
{
	TArray<FTreasure> PickedTreasures;
	if (TreasureClass->NumPicks < 0)
	{
		// TODO: NumPicks가 음수 일경우 확정적으로 N개를 드랍한다.
	}
	else
	{
		// TreasureClass에서 TC 또는 아이템을 하나 뽑는다.
		for (int picksLeft = TreasureClass->NumPicks; picksLeft > 0; picksLeft--)
		{
			int ProbTotal = TreasureClass->FreqNoDrop;
			for (const auto& Treasure : TreasureClass->Treasures)
			{
				ProbTotal += Treasure.Probability;
			}

			int RandomProb = FMath::RandRange(0, ProbTotal);
			int ProbRollSum = 0;
			for (const auto& Treasure : TreasureClass->Treasures)
			{
				ProbRollSum += Treasure.Probability;
				if (ProbRollSum >= RandomProb)
				{
					PickedTreasures.Add(Treasure);
					break;
				}
			}
		}
	}

	TArray<FGameplayTag> PickedItemTypes;
	for (const auto& Treasure : PickedTreasures)
	{
		// 태그가 TC가 아닌 ItemType일 경우 해당 타입을 추가
		if (Treasure.Treasure.MatchesTag(TAG_Item_Type))
		{
			FGameplayTag ItemTypeTag = Treasure.Treasure;
			PickedItemTypes.Add(ItemTypeTag);
		}
		// 태그가 TC일 경우 해당 TC로 다시 ItemType을 뽑는다
		else if (Treasure.Treasure.MatchesTag(TAG_Item_Treasure))
		{
			FGameplayTag TreasureClassTag = Treasure.Treasure;
			TArray<FGameplayTag> ItemTypes = RollTreasureClassPicks(TreasureClass, MonsterLevel);
			PickedItemTypes.Append(ItemTypes);
		}
	}

	return PickedItemTypes;
}

/// <summary>
/// ItemType 태그의 최하위 타입을 하나 뽑는다.
/// </summary>
/// <param name="ItemType">: Item.Type.Armor과 같이 하위 타입일 수도 있고, Item.Type.Armor와 같이 상위 타입일 수도 있다.</param>
FGameplayTag UPrototypeItemGenerator::PickItemType(FGameplayTag ItemType, int32 MonsterLevel)
{
	// ItemType이 Equipment와 같이 상위타입이거나, Armor와 같이 하위 타입일경우 최하위 장비 타입을 뽑는다.
	if (ItemType.MatchesTag(TAG_Item_Type_Equipment))
	{
		return PickItemType_Equipment(ItemType);
	}

	// ItemType이 골드일 경우는 골드를 뽑는다. 골드는 최하위 타입이라 그대로 반환한다.
	if (ItemType.MatchesTag(TAG_Item_Type_Gold))
	{
		return TAG_Item_Type_Gold;
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemType(%s)에 해당하는 최하위 ItemType을 뽑을 수 없습니다."), *ItemType.ToString());
	return FGameplayTag();
}

/// <summary>
/// ItemType에 속해있는 태그 중 가장 하위 태그 하나를 찾는다.
/// </summary>
/// <param name="ItemType">: Item.Type.Equipment 또는 Item.Type.Equipment.XXX </param>
FGameplayTag UPrototypeItemGenerator::PickItemType_Equipment(FGameplayTag ItemType)
{
	// 장비의 최상단 타입인 Equipment일 경우, 하위 타입 중 하나를 랜덤으로 다시 뽑는다.
	// 예를 들면,
	// Item.Type.Equipment
	// - Item.Type.Equipment.Weapon
	// - Item.Type.Equipment.Armor
	// => Weapon, Armor 중 하나를 뽑는다.
	if (ItemType.MatchesTagExact(TAG_Item_Type_Equipment))
	{
		auto TagNode = UGameplayTagsManager::Get().FindTagNode(ItemType);
		auto ChildTagNodes = TagNode->GetChildTagNodes();
		int RandomIndex = FMath::RandRange(0, ChildTagNodes.Num() - 1);
		FGameplayTag PickedType = ChildTagNodes[RandomIndex].Get()->GetCompleteTag();
		return PickItemType_Equipment(PickedType);
	}

	// 상위 타입 중에서 한 가지 최하위 ItemType을 뽑는다.
	// 예를 들면 
	// - Item.Type.Equipment.Armor 
	// -- Item.Type.Equipment.Armor.Helmet
	// -- Item.Type.Equipment.Armor.BodyArmor
	// => Helmet, BodyArmor 중 하나를 뽑는다.
	// 
	// - Item.Type.Equipment.Weapon
	// -- Item.Type.Equipment.Weapon.Sword
	// -- Item.Type.Equipment.Weapon.Axe
	// => Sword, Axe 중 하나를 뽑는다.
	FGameplayTagContainer PickedItemTypes;
	if (ItemType.MatchesTagExact(TAG_Item_Type_Equipment_Armor))
	{
		PickedItemTypes = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment_Armor);
	}
	else if (ItemType.MatchesTagExact(TAG_Item_Type_Equipment_Weapon))
	{
		PickedItemTypes = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment_Weapon);
	}

	// TODO: 1 아이템 타입마다 가중치를 준다. ItemType.txt
	// 예를들면 디아블로2는 특정 클래스 전용 아이템은 가중치가 1, 일반 아이템은 가중치가 3이다
	int RandomIndex = FMath::RandRange(0, PickedItemTypes.Num() - 1);
	FGameplayTag PickedItemType = PickedItemTypes.GetByIndex(RandomIndex);
	return PickedItemType;
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::GenerateItem_Gold(int32 MonsterLevel)
{
	// TODO: 2 골드 획득 배수를 데이터 테이블로 옮긴다.
	// TODO: 1 정예몬스터면 골드 드랍양이 x배로 늘어난다.
	// TODO: 0 난이도에 따라서 골드 드랍양이 y배로 늘어난다.
	// TODO: 2 골드 드랍 +% 스탯에 따라서 골드 드랍양이 늘어난다.
	int32 GoldAmountMultiplier = 6;
	int32 GoldAmountMin = MonsterLevel;
	int32 GoldAmountMax = MonsterLevel * GoldAmountMultiplier - 1;

	TObjectPtr<UPrototypeItem> GoldItem = NewObject<UPrototypeItem>(GetWorld());
	GoldItem->ItemType = TAG_Item_Type_Gold;
	GoldItem->ItemStackAmount = FMath::RandRange(GoldAmountMin, GoldAmountMax);
	GoldItem->ItemBaseName = "골드"; // TODO: 0 데이터테이블의 스트링에서 가져온다.
	GoldItem->Rarity = TAG_Item_Rarity_Normal;
	return GoldItem;
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::GenerateItem_Equipment(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass, FItemType* ItemTypeData)
{
	TObjectPtr<UPrototypeItem> EquipmentItem = NewObject<UPrototypeItem>(GetWorld());
	EquipmentItem->ItemType = ItemType;
	EquipmentItem->ItemStackAmount = 1;

	// 아이템 레벨을 결정한다.
	// TODO: 0 아이템 레벨 값의 범위를 데이터 테이블로 옮긴다. 
	int ItemLevel = FMath::RandRange(MonsterLevel - 1, MonsterLevel + 1);
	EquipmentItem->ItemLevel = ItemLevel;

	// 드롭할 아이템을 결정한다.
	// TODO: Equipment 타입마다 따로 TMap 만들지말구 하나의 TMap에 담기가 가능할까... 가능하면 리팩토링
	// 함수 따로 만드는게 보기엔 나을수도
	if (ItemType.MatchesTag(TAG_Item_Type_Equipment_Armor))
	{
		const auto ArmorData = ItemTypeToArmorData.Find(ItemType);
		if (ArmorData != nullptr)
		{
			TArray<FArmor*> SpawnableArmor;
			for (auto& Armor : *ArmorData)
			{
				// 스폰 불가한 아이템이면 건너뜀
				if (!Armor->BaseData.bSpawnable)
				{
					continue;
				}
				// 드롭할 아이템 레벨보다 아이템데이터의 레벨이 더 높으면 건너뜀
				if (ItemLevel < Armor->BaseData.Level)
				{
					continue;
				}
				SpawnableArmor.Add(Armor);
			}

			int SpawnableArmorCount = SpawnableArmor.Num();
			if (SpawnableArmorCount > 0)
			{
				int RandomIndex = FMath::RandRange(0, SpawnableArmorCount - 1);
				auto ArmorToSpawn = SpawnableArmor[RandomIndex];
				ArmorToSpawn->BaseData.LevelRequirement;
				EquipmentItem->ItemBaseName = ArmorToSpawn->BaseData.InGameName;
				EquipmentItem->Defense = FMath::RandRange(ArmorToSpawn->MinDef, ArmorToSpawn->MaxDef);
			}
		}
	}
	else if (ItemType.MatchesTag(TAG_Item_Type_Equipment_Weapon))
	{
		const auto WeaponData = ItemTypeToWeaponData.Find(ItemType);
		if (WeaponData != nullptr)
		{
			TArray<FWeapon*> SpawnableWeapon;
			for (auto& Weapon : *WeaponData)
			{
				// 스폰 불가한 아이템이면 건너뜀
				if (!Weapon->BaseData.bSpawnable)
				{
					continue;
				}
				// 드롭할 아이템 레벨보다 아이템데이터의 레벨이 더 높으면 건너뜀
				if (ItemLevel < Weapon->BaseData.Level)
				{
					continue;
				}
				SpawnableWeapon.Add(Weapon);
			}

			int SpawnableWeaponCount = SpawnableWeapon.Num();
			if (SpawnableWeaponCount > 0)
			{
				int RandomIndex = FMath::RandRange(0, SpawnableWeaponCount - 1);
				auto WeaponToSpawn = SpawnableWeapon[RandomIndex];
				WeaponToSpawn->BaseData.LevelRequirement;
				EquipmentItem->ItemBaseName = WeaponToSpawn->BaseData.InGameName;
			}
		}
	}
	

	// 아이템의 희귀도를 뽑는다.
	// TODO: 1 ItemType에 ClassSpecific 정보를 추가한다.
	bool bIsClassSpecificItem = false;
	EquipmentItem->Rarity = RollItemRarity(MonsterLevel, ItemLevel, TreasureClass, bIsClassSpecificItem);

	// TODO: 0 고유아이템이거나 세트아이템일 경우 UniqueItems.txt, SetItems.txt에서 뽑아서 드랍한다.
	if (EquipmentItem->Rarity == TAG_Item_Rarity_Unique ||
		EquipmentItem->Rarity == TAG_Item_Rarity_Set)
	{
		// 현재는 고유아이템이나 세트아이템을 구현하지 않음
		EquipmentItem->Rarity = TAG_Item_Rarity_Rare;
	}

	// 희귀도에 따라 접사를 붙인다.
	// TODO: 2 희귀도에 따른 최대 접사 개수를 데이터 테이블로 옮긴다.
	RollItemAffixes(EquipmentItem);

	// 접사에 따라 이름을 정한다.
	FString PrefixName;
	FString SuffixName = " ";
	for (const auto& ItemAffix : EquipmentItem->ItemAffixes)
	{
		if(ItemAffix->AffixType.MatchesTag(TAG_Item_Affix_Type_Prefix))
		{
			PrefixName.Append(ItemAffix->InGameName);
			PrefixName.Append(" ");
		}
		else if (ItemAffix->AffixType.MatchesTag(TAG_Item_Affix_Type_Suffix))
		{
			SuffixName.Append(ItemAffix->InGameName);
			SuffixName.Append(" ");
		}
	}
	EquipmentItem->ItemFullName = PrefixName.Append(EquipmentItem->ItemBaseName).Append(SuffixName);
	
	// 홈 보유 가능 여부에 따라 홈을 추가한다.
	if (ItemTypeData->bCanHaveSockets)
	{
		TArray<int32> PossibleItemLevels;
		int32 MaxSocketCount = 0;
		ItemTypeData->ItemLevelToMaxSocketCount.GetKeys(PossibleItemLevels);
		for (int i = 0; i < PossibleItemLevels.Num(); i++)
		{
			if (ItemLevel >= PossibleItemLevels[i])
			{
				MaxSocketCount = ItemTypeData->ItemLevelToMaxSocketCount[PossibleItemLevels[i]];
			}
		}

		EquipmentItem->SocketCount = FMath::RandRange(0, MaxSocketCount);
	}

	return EquipmentItem;
}

FGameplayTag UPrototypeItemGenerator::RollItemRarity(int32 MonsterLevel, int32 ItemLevel, FTreasureClass* TreasureClass, bool bIsClassSpecificItem)
{
	int32 FreqUnique = TreasureClass->FreqRarities.Contains(TAG_Item_Rarity_Unique) ? TreasureClass->FreqRarities[TAG_Item_Rarity_Unique] : 0;
	int32 FreqSet = TreasureClass->FreqRarities.Contains(TAG_Item_Rarity_Set) ? TreasureClass->FreqRarities[TAG_Item_Rarity_Set] : 0;
	int32 FreqRare = TreasureClass->FreqRarities.Contains(TAG_Item_Rarity_Rare) ? TreasureClass->FreqRarities[TAG_Item_Rarity_Rare] : 0;
	int32 FreqMagic = TreasureClass->FreqRarities.Contains(TAG_Item_Rarity_Magic) ? TreasureClass->FreqRarities[TAG_Item_Rarity_Magic] : 0;
	if (bIsClassSpecificItem)
	{
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_ClassSpecific_Unique, Divisor_ClassSpecific_Unique, MagicFindConstant_Unique, FreqUnique))
		{
			return TAG_Item_Rarity_Unique;
		}
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_ClassSpecific_Set, Divisor_ClassSpecific_Set, MagicFindConstant_Set, FreqSet))
		{
			return TAG_Item_Rarity_Set;
		}
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_ClassSpecific_Rare, Divisor_ClassSpecific_Rare, MagicFindConstant_Rare, FreqRare))
		{
			return TAG_Item_Rarity_Rare;
		}
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_ClassSpecific_Magic, Divisor_ClassSpecific_Magic, MagicFindConstant_Magic, FreqMagic))
		{
			return TAG_Item_Rarity_Magic;
		}
	}
	else
	{
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_Unique, Divisor_Unique, MagicFindConstant_Unique, FreqUnique))
		{
			return TAG_Item_Rarity_Unique;
		}
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_Set, Divisor_Set, MagicFindConstant_Set, FreqSet))
		{
			return TAG_Item_Rarity_Set;
		}
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_Rare, Divisor_Rare, MagicFindConstant_Rare, FreqRare))
		{
			return TAG_Item_Rarity_Rare;
		}
		if (CheckItemRarityCalc(MonsterLevel, ItemLevel, Quality_Magic, Divisor_Magic, MagicFindConstant_Magic, FreqMagic))
		{
			return TAG_Item_Rarity_Magic;
		}
	}
	return TAG_Item_Rarity_Normal;
}

bool UPrototypeItemGenerator::CheckItemRarityCalc(int32 MonsterLevel, int32 ItemLevel, int32 Quality, int32 Divisor, float MagicFindConst, int32 FreqRarity)
{
	// int<->float 형변환 주의!!
	float BaseChance = (Quality - (MonsterLevel - ItemLevel) / Divisor) * 128;
	float MagicFindChance = 500.0f; // TODO: 캐릭터의 Attribute에서 MagicFind를 가져온다.
	float EffectiveMagicFindChance = (MagicFindChance * MagicFindConst) / (MagicFindChance + MagicFindConst); // 0으로 나누는 경우가 있을까??
	float EffectiveChance = BaseChance * 100.0f / (100.0f + EffectiveMagicFindChance);
	float FinalChance = EffectiveChance * (1 - ((float)FreqRarity / 1024));
	float RandomValue = FMath::FRandRange(0, FinalChance - 1);
	UE_LOG(LogTemp, Warning, TEXT("FinalChance(%f) RandomValue(%f)"), FinalChance, RandomValue);
	return RandomValue < 128 ? true : false;
}

void UPrototypeItemGenerator::RollItemAffixes(TObjectPtr<UPrototypeItem> Item)
{
	// 지금은 Item에 대한 Rarity, Level이 다 정해져있다고 가정하고 Item을 매개변수로 받는데, 이렇게 하면 먼저 Rarity, Level을 정해야하므로 종속성이 생겨버림.
	// TODO: 3 Item의 Rarity, Level 등 데이터 종속성을 없애도록 리팩토링
	// FGameplayTagContainer ItemPrefixGroupTags = Item->PrefixGroupTags; // Item클래스에 PrefixGroupTags.HasTag() 함수를 만드는게 나을수도 있음
	// FGameplayTagContainer ItemSuffixGroupTags = Item->SuffixGroupTags;
	FGameplayTag ItemType = Item->ItemType;
	FGameplayTag Rarity = Item->Rarity;
	int32 ItemLevel = Item->ItemLevel;
	// Item->ItemAffixes;

	// 아이템에 붙을 수 있는 접사 갯수를 결정한다.
	int32 MaxAffixCount = 0;
	int32 PrefixCount = 0;
	int32 SuffixCount = 0;
	if (Rarity == TAG_Item_Rarity_Rare)
	{
		// 레어 아이템은 접사가 최소 3개, 최대 6개
		// 접두사 최대 3개, 접미사 최대 3개
		MaxAffixCount = FMath::RandRange(3, 6);
		for (int i = 0; i < MaxAffixCount; i++)
		{
			if (PrefixCount == 3)
			{
				SuffixCount++;
				continue;
			}
			else if (SuffixCount == 3)
			{
				PrefixCount++;
				continue;
			}

			if (FMath::RandBool())
			{
				PrefixCount++;
			}
			else
			{
				SuffixCount++;
			}
		}
	}
	else if (Rarity == TAG_Item_Rarity_Magic)
	{
		MaxAffixCount = FMath::RandRange(1, 3);
		for (int i = 0; i < MaxAffixCount; i++)
		{
			if (PrefixCount == 2)
			{
				SuffixCount++;
				continue;
			}
			else if (SuffixCount == 2)
			{
				PrefixCount++;
				continue;
			}

			if (FMath::RandBool())
			{
				PrefixCount++;
			}
			else
			{
				SuffixCount++;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Roll Item Affix: MaxAffixCount(%d), PrefixCount(%d), SuffixCount(%d)"), MaxAffixCount, PrefixCount, SuffixCount);

	// 접두사를 넣는다.
	AddItemAffixes(Item, TAG_Item_Affix_Type_Prefix, PrefixCount);
	// 접미사를 넣는다.
	AddItemAffixes(Item, TAG_Item_Affix_Type_Suffix, SuffixCount);
	// TEST: 아이템 능력치 적용 테스트 코드
	// 
	/*
	auto Affix = ItemAffixes[0];
	if (Affix)
	{
		auto Modifier = Affix->AffixModifiers[0];
		auto Tag = Modifier.Attribute;
		auto GameplayEffect = ItemAffixAttributes->GetGameplayEffectByAttributeTag(Tag);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GameplayEffect->GetName());
		
		auto PC = GetWorld()->GetFirstPlayerController();
		auto PS = PC->GetPlayerState<APrototypePlayerState>();
		auto ASC = PS->GetAbilitySystemComponent();
		auto EffectContext = ASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(Tag, Modifier.MinValue);
		ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
	}
	*/
}

void UPrototypeItemGenerator::AddItemAffixes(TObjectPtr<UPrototypeItem> Item, FGameplayTag AffixType, int32 AffixCount)
{
	auto ItemAffixes = AffixTypeToAffixData.Find(AffixType);
	if (ItemAffixes == nullptr)
	{
		return;
	}
	// TODO: 게임플레이 태그 사용하는 거 타입제한없어서 너무 헷갈리니까 다 유틸리티함수로 만들어서 따로 빼자?
	for (int i = 0; i < AffixCount; i++)
	{
		int64 TotalFreq = 0;
		TArray<FItemAffix*> SpawnableAffixes;
		for (auto& Affix : *ItemAffixes)
		{
			if (Affix->bOnlySpawnRare && Item->Rarity != TAG_Item_Rarity_Rare)
			{
				// 오직 Rare에서만 나오는 접사인데 아이템이 Rare가 아니라면 건너뛴다.
				continue;
			}
			if (!Affix->SpawnableItemTypes.HasTagExact(Item->ItemType))
			{
				// 해당 접사가 스폰 가능한 ItemType이 아닐 경우 건너뛴다.
				continue;
			}
			if (Item->HasAffixGroupTag(AffixType, Affix->AffixGroup))
			{
				// 해당 접사의 그룹 내 다른 접사가 이미 아이템에 있을 경우 건너뛴다.
				continue;
			}
			SpawnableAffixes.Add(Affix);
			TotalFreq += Affix->Frequency;
		}
		int64 RandomFreq = FMath::RandRange(0LL, TotalFreq);
		int64 RandomFreqSum = 0;
		for (auto& Affix : SpawnableAffixes)
		{
			RandomFreqSum += Affix->Frequency;
			if (RandomFreqSum >= RandomFreq)
			{
				Item->AddAffix(Affix);
				break;
			}
		}
	}
}
