// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffEffectBase.h"
#include "MyPlayCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBuffEffectBase::ApplyEffect(AMyPlayCharacter* Target,UBuffDataBase* buffType)
{
	//nullチェック
	if (!Target || !buffType)return;

	//効果毎に処理を変更する
	switch (buffType->BuffType)
	{
	case EBuffType::AttackUp:
	{
		//攻撃アップ
		Target->P_attack += buffType->Value;
		UE_LOG(LogTemp, Warning, TEXT("Attack Up %d"),Target->P_attack);
	}break;
	case EBuffType::Heal:
	{
		// 2秒ごとに HealTick を呼び出す
		if (Target)
		{
			FTimerDelegate HealDelegate;
			HealDelegate.BindUFunction(this, FName("HealTick"), Target,buffType);
			GetWorld()->GetTimerManager().SetTimer(HealTimer, HealDelegate, 2.0f, true);
		}
	} break;
	case EBuffType::SpeedUp:
	{
		//スピードアップ
		Target->P_speed = buffType->Value;
		UE_LOG(LogTemp, Warning, TEXT("Speed Up %f"), Target->P_speed);
	}break;
	default:
		break;
	}
}

void UBuffEffectBase::HealTick(AMyPlayCharacter* Target,UBuffDataBase* Buff)
{
	if (!Target) return;

	Target->P_hp = FMath::Clamp(Target->P_hp + Buff->Value, 0.0f, Target->P_max_hp);
	UE_LOG(LogTemp, Warning, TEXT("Heal Tick: %d"), Target->P_hp);
}
