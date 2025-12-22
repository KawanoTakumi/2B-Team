// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class KITAKURPG_API ULevelUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BluePrintCallable)
	void OnBuffSelected();//ƒoƒt‘I‘ðŽžŒÄ‚Î‚ê‚é

	UFUNCTION(BluePrintCallable)
	void SelectBuff(UBuffDataBase* Buff);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <class UBuffDataBase* > BuffOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SE")
	USoundBase* hit_se;
};