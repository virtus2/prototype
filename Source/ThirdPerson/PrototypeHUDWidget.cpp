// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeHUDWidget.h"

#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>


void UPrototypeHUDWidget::UpdateHealth(float CurrentHealthPoint, float MaxHealthPoint)
{
    float percent = CurrentHealthPoint / (MaxHealthPoint + 0.00001f); 
    HealthPointProgressBar->SetPercent(percent);
    
    HealthPointTextBlock->SetText(FText::AsNumber(CurrentHealthPoint));
}
