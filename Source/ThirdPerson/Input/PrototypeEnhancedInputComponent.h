// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "PrototypeInputConfig.h"
#include "PrototypeEnhancedInputComponent.generated.h"

class UPrototypeInputConfig;

UCLASS()
class THIRDPERSON_API UPrototypeEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:

	/* InputConfig에서 정의한 <InputAction, InputTag> 쌍을 EnhancedInput에 바인딩한다. */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UPrototypeInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UPrototypeEnhancedInputComponent::BindAbilityActions(const UPrototypeInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	if (!IsValid(InputConfig))
	{
		return;
	}

	for (const FPrototypeInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
