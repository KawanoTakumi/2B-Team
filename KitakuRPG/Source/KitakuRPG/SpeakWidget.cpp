// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeakWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void USpeakWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Text_Dialog) {
		Text_Dialog->SetText(FText::GetEmpty());
	}
}

void USpeakWidget::SetLineImmediate(const FText& InText)
{
	GetWorld()->GetTimerManager().ClearTimer(TypeTimer);
	bTypewriting = false;

	FullText = InText;
	if (Text_Dialog) {
		Text_Dialog->SetText(FullText);
	}
}

void USpeakWidget::PlayLineTypewriter(const FText& InText, float CharInterval)
{
	GetWorld()->GetTimerManager().ClearTimer(TypeTimer);

	FullText = InText;
	FullTextStr = FullText.ToString(); // 日本語OK（UTF-16→UTF-8は内部管理される）

	CurrentIndex = 0;
	Interval = FMath::Max(0.001f, CharInterval);
	bTypewriting = true;

	if (Text_Dialog)
	{
		Text_Dialog->SetText(FText::GetEmpty());
	}

	// 最初のティックを開始
	GetWorld()->GetTimerManager().SetTimer(
		TypeTimer, this, &USpeakWidget::AdvanceTypewriter, Interval, true);

}

void USpeakWidget::SkipTypewriter()
{
	if (!bTypewriting) return;

	GetWorld()->GetTimerManager().ClearTimer(TypeTimer);
	bTypewriting = false;

	if (Text_Dialog) {
		Text_Dialog->SetText(FullText);
	}

}

void USpeakWidget::AdvanceTypewriter()
{
	if (!Text_Dialog) {
		SkipTypewriter();
		return;
	}

	if (CurrentIndex >= FullTextStr.Len()) {
		SkipTypewriter();
		return;
	}

	const FString Sub = FullTextStr.Left(++CurrentIndex);
	Text_Dialog->SetText(FText::FromString(Sub));
}