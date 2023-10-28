// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "PrototypeGameItemActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPrototypeAbilitySet;

USTRUCT(BlueprintType)
struct FAttributeStatMinMaxValue
{
	GENERATED_BODY()
	/*
	 * GAS의 Attribute에 적용할 최소값, 최대값을 저장하는 구조체
	 * 
	 * 아이템을 생성할때는 MinValue, MaxValue 사이의 값을 무작위로 골라 ActualValue에 저장한다.
	 * 이후 이 값을 Gameplay Effect의 Modifier - SetByCaller에 넘겨서
	 * 해당 Effect가 적용될 때(ex: 아이템 장착 시) Attribute값을 변경한다.
	 * 
	 * 아이템 데이터를 저장/로드할때는 ActualValue를 사용한다.
	 * 사실상 런타임에는 ActualValue만 사용해서 계산하게 될 것 같다.
	 *  
	 * SetByCaller로 런타임에 일정 범위값 내 무작위 값을 Gameplay Effect에 넘겨주는건 가능하다.
	 * 하지만 이건 일회성으로 값을 지정해주는 느낌이다. 게임 아이템 데이터를 저장/로드하기 위해서
	 * 구조체 멤버변수에 값을 보관해두고, 이 값을 SetByCaller에도 사용하고 아이템데이터 저장에도 사용하는 방식으로 우회했다.
	 *  
	 */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MinValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxValue;

	UPROPERTY(VisibleAnywhere)
	int32 ActualValue;

public:

};

USTRUCT(BlueprintType)
struct FPrototypeGameItemAffix
{
	GENERATED_BODY()
	/*
	 *  아이템에 부여할 수 있는 접사 데이터 구조체
	 *  Diablo II의 Magicprefix.txt와 Magicsuffix.txt 를 참고했다.
	 */

	// TODO: 접두사와 접미사를 따로 나눌지 아니면 단순히 FGameplayTag로 구분할 지 생각해봐야한다.
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AffixName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AffixDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAttributeStatMinMaxValue> AttributeMinMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAppearOnlyOnMagicItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Frequency;

	/* Minimum Affix Level of an item that may spawn with this affix. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	/* Maximum Affix Level of an itemthat may spawn with this affix. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel;

	/* Minimum required character level for using item with this affix. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelRequirement;
};

USTRUCT(BlueprintType)
struct FPrototypeGameItemAffixDataTable : public FTableRowBase
{
	GENERATED_BODY()

	/*
	 * 기획자(디자이너)가 접사 데이터를 추가, 제거, 편집할 수 있는 데이터 테이블
	 * 이 테이블에 있는 데이터에 따라 게임내에서 생성되는 아이템의 옵션(접사)가 결정된다.
	 */

	// TODO: 게임 시작 시 데이터 테이블을 로드하고, 아이템 생성 시 사용하는 클래스가 필요하다.
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPrototypeGameItemAffix GameItemAffix;
};

USTRUCT(BlueprintType)
struct FPrototypeGameItemData
{
	GENERATED_BODY()
	/*
	 * 인게임(런타임)에서 사용하는 게임 아이템 데이터
	 */
	// TODO: UObject로 만드는게 나은가?
	// - 인벤토리내에 들어간 아이템 인스턴스(GameItemInventoryEntry??)
	// - 레벨에 배치된 아이템 인스턴스(GameItemActor??)
	// 위의 클래스들이 이 데이터에 대한 포인터를 갖게하고, 
	// 인벤토리나 레벨에서 배치될땐 어떻게 출력되어야 할지 각자 결정하게 해야함
	// 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPrototypeGameItemAffix> GameItemAffix;
};

UCLASS()
class THIRDPERSON_API APrototypeGameItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrototypeGameItemActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:

public:

	UPROPERTY(EditAnywhere, Category = "Prototype|Game Item")
	FPrototypeGameItemData GameItemData;

protected:
	/* 게임 아이템의 구(Sphere) 컴포넌트 */
	UPROPERTY(EditAnywhere, Category = "Prototype|Game Item")
	TObjectPtr<USphereComponent> SphereComponent;
	
	/* 게임 아이템의 스태틱 메쉬 */
	UPROPERTY(EditAnywhere, Category = "Prototype|Game Item")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

private:
	// TODO: 삭제? 보류? Attribute는 Affix로 변경할건데, Ability는 이거로 부여하는게 편할수도 있음. 
	/* 아이템 획득 시 캐릭터에게 부여할 어빌리티 */
	UPROPERTY(EditAnywhere, Category = "Prototype|Game Item")
	TObjectPtr<UPrototypeAbilitySet> GameItemAbilities;
};
