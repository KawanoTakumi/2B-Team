#include "TitleHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h" // ’Ç‰Á: UUserWidget ‚ÌŠ®‘S‚ÈŒ^î•ñ‚ğæ“¾

ATitleHUD::ATitleHUD()
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBPClass(TEXT("/Game/egawa/UI/BPW_TitleMenu"));
	//if (WidgetBPClass.Succeeded())
	//{
	//	WidgetClass = WidgetBPClass.Class;
	//}
}

void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (WidgetClass && PlayerController)
	{
		// Widget‚ğì¬‚·‚é
		UserWidget = UWidgetBlueprintLibrary::Create(GetWorld(), WidgetClass, PlayerController);

		// Viewport‚É’Ç‰Á‚·‚é
		UserWidget->AddToViewport(0);

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