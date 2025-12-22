// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
class UButton;
UCLASS()
class KITAKURPG_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonPlay;

	UFUNCTION()
	void OnButtonPlayClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonQuit;

	UFUNCTION()
	void OnButtonQuitClicked();

public:
	FString name;

};
