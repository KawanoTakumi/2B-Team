#include "TitleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h" // ’Ç‰Á: UUserWidget ‚ÌŠ®‘S‚ÈŒ^î•ñ‚ðŽæ“¾

ATitleHUD::ATitleHUD()
{
}

void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (WidgetClass && PlayerController)
	{
		// Widget‚ðì¬‚·‚é
		UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		// Viewport‚É’Ç‰Á‚·‚é
		UserWidget->AddToViewport(0);

		// MouseCursor‚ð•\Ž¦‚·‚é
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