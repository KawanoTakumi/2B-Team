// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/ProgressBar.h"
#include "MyPlayCharacter.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	Level_Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level_Text")));
}

 void UMyUserWidget::SetHPPercent(float Per)
{
	if (HPBar)
	{
		HPBar->SetPercent(FMath::Clamp(Per, 0.0f, 1.0f));
	}
	
}

 void UMyUserWidget::SetLevel(int Level)
 {
	 if (Level_Text)
	 {
		 Level_Text->SetText(FText::FromString(FString::Printf(TEXT("Lv.%d"),Level)));

	 }
 }