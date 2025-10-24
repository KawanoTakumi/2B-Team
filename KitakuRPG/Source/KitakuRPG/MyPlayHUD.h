// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyUserWidget.h"
#include "MyPlayHUD.generated.h"

/**
 * 
 */
UCLASS()
class KITAKURPG_API AMyPlayHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(editDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MyUserWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UMyUserWidget* MyUserWidgetInstance;

	void UpdateHPBar(float CurrentHP, float MaxHP);
};