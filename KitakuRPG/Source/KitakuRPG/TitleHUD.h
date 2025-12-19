// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TitleHUD.generated.h"

UCLASS()
class KITAKURPG_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATitleHUD();

protected:
	virtual void BeginPlay() override;

public:
	void CloseTitle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> WidgetClass;

private:

	UPROPERTY()
	UUserWidget* UserWidget;
};
