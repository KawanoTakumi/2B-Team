// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayHUD.h"
#include "MyUserWidget.h"
//#include "Blueprint/MyUserWidget.h"

void AMyPlayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MyUserWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetMyUserWidget"));
		UE_LOG(LogTemp, Warning, TEXT("MyUserWidgetClass: %s"), *MyUserWidgetClass->GetName());
		MyUserWidgetInstance = CreateWidget<UMyUserWidget>(GetWorld(), MyUserWidgetClass);
		
		if (MyUserWidgetInstance)
		{
			MyUserWidgetInstance->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("WIDGET"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not WIDGET"));

		}
	}

}

void AMyPlayHUD::UpdateHPBar(float CurrentHP, float MaxHP)
{
	if (MyUserWidgetInstance)
	{
		float Percent = CurrentHP / MaxHP;
		MyUserWidgetInstance->SetHPPercent(Percent);
	}
}
