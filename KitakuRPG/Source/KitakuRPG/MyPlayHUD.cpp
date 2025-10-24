// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayHUD.h"
#include "MyUserWidget.h"
//#include "Blueprint/MyUserWidget.h"

void AMyPlayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MyUserWidgetClass)
	{
		MyUserWidgetInstance = CreateWidget<UMyUserWidget>(GetWorld(), MyUserWidgetClass);
		UE_LOG(LogTemp, Warning, TEXT("get user widget"));
	}
	if (MyUserWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Name: %s"), *MyUserWidgetInstance->GetName());
		MyUserWidgetInstance->AddToViewport();
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