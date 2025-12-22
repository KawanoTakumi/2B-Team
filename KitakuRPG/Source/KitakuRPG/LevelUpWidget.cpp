// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayCharacter.h"
#include "GameInstanceValue.h"


//バフ選択時に呼ぶ
void ULevelUpWidget::OnBuffSelected()
{

    //SEを再生
    if(hit_se)
        UGameplayStatics::PlaySound2D(this, hit_se);
	//ゲームを再開
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	//入力モードをUIのみにする
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly InputOnry;
	PC->SetInputMode(InputOnry);
	PC->bShowMouseCursor = false;
	//ウィジェット非表示
	RemoveFromParent();
}
void ULevelUpWidget::SelectBuff(UBuffDataBase* SelectedBuff)
{
    AMyPlayCharacter* Player = Cast<AMyPlayCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player && SelectedBuff)
    {
        Player->AddBuff(SelectedBuff); // 攻撃力アップなどの効果を適用
    }

    UGameplayStatics::SetGamePaused(GetWorld(), false);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }

    RemoveFromParent();
}
