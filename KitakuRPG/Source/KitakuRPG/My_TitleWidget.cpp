// Fill out your copyright notice in the Description page of Project Settings.


#include "My_TitleWidget.h"
#include "Components/Button.h"
#include "Kismet/GamePlayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMy_TitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UMy_TitleWidget::OnButtonPlayClicked);

	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UMy_TitleWidget::OnButtonQuitClicked);

}

void UMy_TitleWidget::OnButtonPlayClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// UIを閉じる
	RemoveFromParent();

	// 入力モードをゲーム専用に
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;

	// レベル遷移
	UGameplayStatics::OpenLevel(this, FName("Stage_1"));

}

void UMy_TitleWidget::OnButtonQuitClicked()
{
	// PlayerControllerを取得する
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// ゲームを終了する
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}