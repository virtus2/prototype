// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItemGenerator.h"

#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"

#include "ThirdPerson/PrototypeGameplayTags.h"
#include "ThirdPerson/Data/TreasureClass.h"
#include "ThirdPerson/Data/ItemAffix.h"
#include "ThirdPerson/Data/ItemType.h"
#include "ThirdPerson/Item/PrototypeItem.h"
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

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemAffixDataTableClass(TEXT("/Game/Prototype/Data/DT_ItemAffix.DT_ItemAffix"));
	if (IsValid(ItemAffixDataTableClass.Object))
	{
		ItemAffixDataTable = ItemAffixDataTableClass.Object;

		const FString Context;
		ItemAffixDataTable->GetAllRows<FItemAffix>(Context, ItemAffixes);
		// TODO: 접두사, 접미어를 구분해서 TArray에 넣는다.
		for (int i = 0; i < ItemAffixes.Num(); i++)
		{

		}
	}

	static ConstructorHelpers::FObjectFinder<UAttributeGameplayEffectMap> AttributeMap(TEXT("/Game/Prototype/Data/DA_AttributeGameplayEffectMap.DA_AttributeGameplayEffectMap"));
	if (IsValid(AttributeMap.Object))
	{
		ItemAffixAttributes = AttributeMap.Object;
	}
}

TArray<TObjectPtr<UPrototypeItem>> UPrototypeItemGenerator::GenerateItems(FGameplayTag TreasureClass, int32 MonsterLevel)
{
	const auto& GeneratedItems = RollTreasureClassPicks(TreasureClass, MonsterLevel);
	UE_LOG(LogTemp, Warning, TEXT("Generated Items for TreasureClass(%s)"), *TreasureClass.ToString());
	for (const auto& Item : GeneratedItems)
	{
		Item->DebugLog();
	}
	return GeneratedItems;
}

TArray<TObjectPtr<UPrototypeItem>> UPrototypeItemGenerator::RollTreasureClassPicks(FGameplayTag TreasureClass, int32 MonsterLevel)
{
	TArray<TObjectPtr<UPrototypeItem>> ResultItems;
	if (!IsValid(TreasureClassDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable(TreasureClass) is not valid."));
		return ResultItems;
	}

	const FString Context;
	FTreasureClass* TC = TreasureClassDataTable->FindRow<FTreasureClass>(TreasureClass.GetTagName(), Context);
	if (TC == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TreasureClass데이터 테이블에 (%s)에 해당하는 데이터가 없습니다."), *TreasureClass.ToString());
		return ResultItems;
	}

	TArray<FTreasure> PickedTreasures;
	// TODO: NumPicks가 음수 일경우 확정적으로 N개를 드랍한다.
	if (TC->NumPicks < 0)
	{

	}
	else
	{
		// TreasureClass에서 TC 또는 아이템을 하나 뽑는다.
		for (int picksLeft = TC->NumPicks; picksLeft > 0; picksLeft--)
		{
			int ProbTotal = TC->FreqNoDrop;
			for (const auto& Treasure : TC->Treasures)
			{
				ProbTotal += Treasure.Probability;
			}

			int RandomProb = FMath::RandRange(0, ProbTotal);
			int ProbRollSum = 0;
			for (const auto& Treasure : TC->Treasures)
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

	for (const auto& Treasure : PickedTreasures)
	{
		// TreasureClass가 아닌 아이템 타입일 경우 해당 아이템을 드롭
		if (Treasure.Treasure.MatchesTag(TAG_Item_Type))
		{
			FGameplayTag ItemTypeTag = Treasure.Treasure;
			TObjectPtr<UPrototypeItem> Item = PickItem(ItemTypeTag, MonsterLevel, TC);
			if (IsValid(Item))
			{
				ResultItems.Add(Item);
			}
		}
		// TreasureClass일 경우 해당 TC로 다시 드랍될 아이템을 뽑는다
		else if (Treasure.Treasure.MatchesTag(TAG_Item_Treasure))
		{
			FGameplayTag TreasureClassTag = Treasure.Treasure;
			TArray<TObjectPtr<UPrototypeItem>> Items = RollTreasureClassPicks(TreasureClassTag, MonsterLevel);
			for (const auto& Item : Items)
			{
				ResultItems.Add(Item);
			}
		}
	}

	return ResultItems;
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::PickItem(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass)
{
	if (ItemType.MatchesTag(TAG_Item_Type_Equipment))
	{
		return PickItem_Equipment(ItemType, MonsterLevel, TreasureClass);
	}
	
	if (ItemType.MatchesTag(TAG_Item_Type_Gold))
	{
		return PickItem_Gold(MonsterLevel);
	}

	UE_LOG(LogTemp, Warning, TEXT("아이템 타입(%s)에 해당하는 아이템을 생성할 수 없습니다."), *ItemType.ToString());
	return nullptr;
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::PickItem_Equipment(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass)
{
	// 장비의 최상단 타입인 Equipment일 경우, 하위 타입 중 하나를 랜덤으로 다시 뽑는다.
	if (ItemType.MatchesTagExact(TAG_Item_Type_Equipment))
	{
		auto TagNode = UGameplayTagsManager::Get().FindTagNode(ItemType);
		auto ChildTagNodes = TagNode->GetChildTagNodes();
		int RandomIndex = FMath::RandRange(0, ChildTagNodes.Num() - 1);
		FGameplayTag PickedType = ChildTagNodes[RandomIndex].Get()->GetCompleteTag();
		return PickItem_Equipment(PickedType, MonsterLevel, TreasureClass);
	}
	
	// 제네릭 타입 중에서 한 가지 아이템 타입을 뽑는다.
	FGameplayTagContainer PickedItems;
	if (ItemType.MatchesTagExact(TAG_Item_Type_Equipment_Armor))
	{
		PickedItems = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment_Armor);
	}
	else if (ItemType.MatchesTagExact(TAG_Item_Type_Equipment_Weapon))
	{
		PickedItems = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment_Weapon);
	}

	// TODO: 아이템 타입마다 가중치를 준다. ItemType.txt
	// 예를들면 디아블로2는 특정 클래스 전용 아이템은 가중치가 1, 일반 아이템은 가중치가 3이다
	int RandomIndex = FMath::RandRange(0, PickedItems.Num() - 1);
	FGameplayTag PickedItem = PickedItems.GetByIndex(RandomIndex);
	
	return GenerateItem_Equipment(PickedItem, MonsterLevel, TreasureClass);
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::PickItem_Gold(int32 MonsterLevel)
{
	return GenerateItem_Gold(MonsterLevel);
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::GenerateItem_Equipment(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass)
{
	if (!IsValid(ItemTypeDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable(ItemType) is not valid."));
		return nullptr;
	}

	const FString Context;
	FItemType* ItemTypeData = ItemTypeDataTable->FindRow<FItemType>(ItemType.GetTagName(), Context);
	if (ItemTypeData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemType데이터 테이블에 (%s)에 해당하는 데이터가 없습니다."), *ItemType.ToString());
		return nullptr;
	} 

	// 장비 아이템 생성
	TObjectPtr<UPrototypeItem> EquipmentItem = NewObject<UPrototypeItem>(GetWorld());
	EquipmentItem->ItemType = ItemType;
	EquipmentItem->ItemStackAmount = 1;

	// 아이템 레벨을 결정한다.
	// TODO: 아이템 레벨 값의 범위를 데이터 테이블로 옮긴다.
	int ItemLevel = FMath::RandRange(MonsterLevel - 1, MonsterLevel + 1);
	EquipmentItem->ItemLevel = ItemLevel;

	// 제일 높은 희귀도인 Unique부터 희귀도 랜덤뽑기
	// TODO: ItemType에 ClassSpecific 정보를 추가한다.
	EquipmentItem->Rarity = RollItemRarity(TAG_Item_Rarity_Unique, MonsterLevel, ItemLevel, TreasureClass, false);

	// TODO: 고유아이템이거나 세트아이템일 경우 UniqueItems.txt, SetItems.txt에서 뽑아서 드랍한다.
	if (EquipmentItem->Rarity == TAG_Item_Rarity_Unique ||
		EquipmentItem->Rarity == TAG_Item_Rarity_Set)
	{
		// 현재는 고유아이템이나 세트아이템을 구현하지 않음
		EquipmentItem->Rarity = TAG_Item_Rarity_Rare;
	}


	// 희귀도에 따라 접사를 붙인다.
	// TODO: 희귀도에 따른 최대 접사 개수를 데이터 테이블로 옮긴다.
	RollItemAffixes(EquipmentItem);

	// TODO: 접사에 따라 이름을 정한다.
	
	// TODO: 홈 보유 가능 여부에 따라 홈 갯수 추가한다.
	if (ItemTypeData->bCanHaveSockets)
	{
	}

	return EquipmentItem;
}

FGameplayTag UPrototypeItemGenerator::RollItemRarity(FGameplayTag Rarity, int32 MonsterLevel, int32 ItemLevel, FTreasureClass* TreasureClass, bool bIsClassSpecific)
{
	if (TreasureClass == nullptr)
	{
		return TAG_Item_Rarity_Normal;
	}

	int32 Quality;
	int32 Divisor;
	float MagicFindConstant;
	if (bIsClassSpecific)
	{
		if (Rarity.MatchesTag(TAG_Item_Rarity_Unique))
		{
			Quality = Quality_ClassSpecific_Unique;
			Divisor = Divisor_ClassSpecific_Unique;
			MagicFindConstant = MagicFindConstant_Unique;
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Set))
		{
			Quality = Quality_ClassSpecific_Set;
			Divisor = Divisor_ClassSpecific_Set;
			MagicFindConstant = MagicFindConstant_Set;
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Rare))
		{
			Quality = Quality_ClassSpecific_Rare;
			Divisor = Divisor_ClassSpecific_Rare;
			MagicFindConstant = MagicFindConstant_Rare;
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Magic))
		{
			Quality = Quality_ClassSpecific_Magic;
			Divisor = Divisor_ClassSpecific_Magic;
		}
	}
	else
	{
		if (Rarity.MatchesTag(TAG_Item_Rarity_Unique))
		{
			Quality = Quality_Unique;
			Divisor = Divisor_Unique;
			MagicFindConstant = MagicFindConstant_Unique;
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Set))
		{
			Quality = Quality_Set;
			Divisor = Divisor_Set;
			MagicFindConstant = MagicFindConstant_Set;
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Rare))
		{
			Quality = Quality_Rare;
			Divisor = Divisor_Rare;
			MagicFindConstant = MagicFindConstant_Rare;
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Magic))
		{
			Quality = Quality_Magic;
			Divisor = Divisor_Magic;
		}
	}
	float BaseChance = (Quality - (MonsterLevel - ItemLevel) / Divisor) * 128;
	// 매찬 적용
	float MagicFindChance = 500.0f; // TODO: 캐릭터의 Attribute에서 MagicFind를 가져온다.
	float EffectiveMagicFindChance = (MagicFindChance * MagicFindConstant) / (MagicFindChance + MagicFindConstant); // TODO: 0으로 나누는 경우가 있으면 조심해야됨
	float EffectiveChance = BaseChance * 100.0f / (100.0f + EffectiveMagicFindChance);
	float QualityConstant;
	if (TreasureClass->FreqRarities.Contains(Rarity))
	{
		QualityConstant = (float)*TreasureClass->FreqRarities.Find(Rarity);
	}
	else
	{
		QualityConstant = 0;
	}

	float FinalChance = EffectiveChance * (1 - (QualityConstant / 1024));
	float RandomValue = FMath::FRandRange(0, FinalChance - 1);
	UE_LOG(LogTemp, Warning, TEXT("Rarity(%s) Check: MF Chance(%f), Eff Chance(%f), Final Chance(%f)"), *Rarity.ToString(), MagicFindChance, EffectiveChance, FinalChance);
	if (RandomValue > 128)
	{
		if (Rarity.MatchesTag(TAG_Item_Rarity_Unique))
		{
			return RollItemRarity(TAG_Item_Rarity_Set, MonsterLevel, ItemLevel, TreasureClass, bIsClassSpecific);
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Set))
		{
			return RollItemRarity(TAG_Item_Rarity_Rare, MonsterLevel, ItemLevel, TreasureClass, bIsClassSpecific);
		}
		else if (Rarity.MatchesTag(TAG_Item_Rarity_Rare))
		{
			return RollItemRarity(TAG_Item_Rarity_Magic, MonsterLevel, ItemLevel, TreasureClass, bIsClassSpecific);
		}
	}
	else
	{
		return Rarity;
	}

	return TAG_Item_Rarity_Normal;
}

void UPrototypeItemGenerator::RollItemAffixes(TObjectPtr<UPrototypeItem> Item)
{
	// 아이템에 붙을 수 있는 접사 갯수를 결정한다.
	int32 MaxAffixCount = 0;
	int32 PrefixCount = 0;
	int32 SuffixCount = 0;
	if (Item->Rarity == TAG_Item_Rarity_Rare)
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
	else if (Item->Rarity == TAG_Item_Rarity_Magic)
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

	// TODO: 접두사와 접미사를 갯수에 맞게 넣는다.
	for (int i = 0; i < MaxAffixCount; i++)
	{
	}

	// 테스트 코드
	// Item->AddAffix(ItemAffixes[0]);
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
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::GenerateItem_Gold(int32 MonsterLevel)
{
	// TODO: 골드 획득 배수를 데이터 테이블로 옮긴다.
	// TODO: 정예몬스터면 골드 드랍양이 x배로 늘어난다.
	// TODO: 난이도에 따라서 골드 드랍양이 y배로 늘어난다.
	// TODO: 골드 드랍 +% 스탯에 따라서 골드 드랍양이 늘어난다.
	int32 GoldAmountMultiplier = 6;
	int32 GoldAmountMin = MonsterLevel;
	int32 GoldAmountMax = MonsterLevel * GoldAmountMultiplier - 1;
	
	TObjectPtr<UPrototypeItem> GoldItem = NewObject<UPrototypeItem>(GetWorld());
	GoldItem->ItemType = TAG_Item_Type_Gold;
	GoldItem->ItemStackAmount = FMath::RandRange(GoldAmountMin, GoldAmountMax);
	GoldItem->ItemName = "골드"; // TODO: 데이터테이블의 스트링에서 가져온다.
	GoldItem->Rarity = TAG_Item_Rarity_Normal;
	return GoldItem;
}
