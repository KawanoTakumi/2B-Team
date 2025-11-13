// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffDataBase.h"
#include "BuffEffectBase.generated.h"

class AMyPlayCharacter;
/**
 * 
 */
UCLASS(Blueprintable)
class KITAKURPG_API UBuffEffectBase : public UObject
{
	GENERATED_BODY()
	
public:

	FTimerHandle HealTimer;//回復のタイマー
	FTimerHandle PoisonTimer;//毒のタイマー
	FTimerHandle ATK_down_Timer;//攻撃力減少タイマー
	FTimerHandle SPEED_down_Timer;//速度減少タイマー


	UFUNCTION(BlueprintCallable)
	void ApplyEffect(AMyPlayCharacter* target,UBuffDataBase* bufftype);
	//一秒毎の回復処理
	UFUNCTION()
	void HealTick(AMyPlayCharacter* Target);
	UFUNCTION()
	//一秒毎のダメージ処理
	void PoisonTick(AMyPlayCharacter* Target,UBuffDataBase* base);
	UFUNCTION()
	void StopPoisonTimer();
	
	UFUNCTION()
	void ATK_downTick(AMyPlayCharacter* Target, UBuffDataBase* base);
	UFUNCTION()
	void SPEED_downTick(AMyPlayCharacter* Target, UBuffDataBase* base);
};