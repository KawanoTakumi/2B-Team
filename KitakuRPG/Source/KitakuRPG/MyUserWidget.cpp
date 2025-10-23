// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/ProgressBar.h"

void UMyUserWidget::SetHPPercent(float Percent)
{
	if (HPBar)
	{
		//0.0～1.0の範囲でHPバーを更新
		HPBar->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}

