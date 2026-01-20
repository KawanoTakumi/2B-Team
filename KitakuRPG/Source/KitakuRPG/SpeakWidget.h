//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpeakWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class KITAKURPG_API USpeakWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//文章を即時に反映
	UFUNCTION(BlueprintCallable)
	void SetLineImmediate(const FText& InText);

	//タイプライターみたいに１文字ずつ出る
	UFUNCTION(BlueprintCallable)
	void PlayLineTypewriter(const FText& InText, float CharInterval = 0.03f);

	//タイプライターをスキップ
	UFUNCTION(BlueprintCallable)
	void SkipTypewriter();

	//表示中かどうか
	UFUNCTION(BlueprintPure)
	bool IsTypewriting() const { return bTypewriting; }

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Dialog;

private:
	void AdvanceTypewriter();

	FText FullText;
	FString FullTextStr;
	int32 CurrentIndex = 0;
	FTimerHandle TypeTimer;
	bool bTypewriting = false;
	float Interval = 0.03f;


};
