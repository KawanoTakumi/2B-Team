// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"

#include "My_TitleWidget.h"
#include "GameInstanceValue.h"
#include "Components/Button.h"
#include "Kismet/GamePlayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonPlay->OnClicked.AddUniqueDynamic(this, &UTitleWidget::OnButtonPlayClicked);

	ButtonQuit->OnClicked.AddUniqueDynamic(this, &UTitleWidget::OnButtonQuitClicked);

}

void UTitleWidget::OnButtonPlayClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	RemoveFromParent();

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
	UGameInstanceValue* value = Cast<UGameInstanceValue>(GetWorld()->GetGameInstance());
	name = value->levelname;
	UGameplayStatics::OpenLevel(this, FName(name));
}

void UTitleWidget::OnButtonQuitClicked()
{
	UGameplayStatics::OpenLevel(this, FName("Title"));
}