#include "TitleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h" // ’Ç‰Á: UUserWidget ‚ÌŠ®‘S‚ÈŒ^î•ñ‚ğæ“¾

void ATitleHUD::BeginPlay()
{
	FString Path = TEXT("/Game/egawa/UI/BPW_TitleMenu.BPW_TitleMenu_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (WidgetClass && PlayerController)
	{
		// Widget‚ğì¬‚·‚é
		UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		// Viewport‚É’Ç‰Á‚·‚é
		UserWidget->AddToViewport();

		// MouseCursor‚ğ•\¦‚·‚é
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, UserWidget, EMouseLockMode::DoNotLock, true, false);
		PlayerController->SetShowMouseCursor(true);

	}
}

void ATitleHUD::CloseTitle()
{
	if (UserWidget)
	{
		UserWidget->RemoveFromParent();
		UserWidget = nullptr;
	}
}