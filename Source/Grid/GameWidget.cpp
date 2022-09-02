// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidget.h"

#include "Components/TextBlock.h"

bool UGameWidget::Initialize()
{
	return Super::Initialize();
}

void UGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	State->SetText(FText::FromString("Ref"));
}

void UGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	const FString Health = FString::SanitizeFloat(3090);
	State->SetText(FText::FromString("Game Over"));
	const FText Message = FText::FromString("Life: " + Health);
	Life->SetText(Message);
}
