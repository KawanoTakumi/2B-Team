// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "MyUserWidget.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class KITAKURPG_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//C++から呼び出せる関数
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetHPPercent(float Percent);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetLevel(int Level);

	void NativeConstruct() override;
protected:
	//UMGのProgressBarをバインド
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Level_Text;

};