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

	RemoveFromParent();

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;

	UGameplayStatics::OpenLevel(this, FName("/Game/Stage/Stage_1"));
}

void UMy_TitleWidget::OnButtonQuitClicked()
{
	// PlayerController‚ğæ“¾‚·‚é
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// ƒQ[ƒ€‚ğI—¹‚·‚é
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}