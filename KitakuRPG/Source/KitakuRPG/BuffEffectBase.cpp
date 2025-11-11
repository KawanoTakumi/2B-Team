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
	}break;
	case EBuffType::Heal:
	{
		// 1秒ごとに HealTick を呼び出す
		if (Target)
		{
			Target->heal_by_time += buffType->Value;//自然回復力を強化
			FTimerDelegate HealDelegate;
			HealDelegate.BindUFunction(this, FName("HealTick"), Target);
			GetWorld()->GetTimerManager().SetTimer(HealTimer, HealDelegate, 1.0f, true);
		}
	} break;
	case EBuffType::SpeedUp:
	{
		//スピードアップ
		Target->P_speed += buffType->Value;
	}break;
	case EBuffType::AttackDown:
	{
		//攻撃力ダウン
		if (Target)
		{
			FTimerDelegate ATK_stop_delegate;



		}

	}break;
	case EBuffType::Poison:
	{
		//毒
		if (Target)
		{
			//毒デリゲート
			FTimerDelegate PoisonDelegate;
			PoisonDelegate.BindUFunction(this, FName("PoisonTick"), Target);
			GetWorld()->GetTimerManager().SetTimer(HealTimer, PoisonDelegate, 1.0f, true);


			//毒終了デリゲート
			FTimerDelegate PoisonStopDelegate;

		}
		

	}break;
	default:
		break;
	}
}
//回復処理
void UBuffEffectBase::HealTick(AMyPlayCharacter* Target)
{
	if (!Target) return;

	Target->P_hp = FMath::Clamp(Target->P_hp + Target->heal_by_time, 0.0f, Target->P_max_hp);
}
//毒処理
void UBuffEffectBase::PoisonTick(AMyPlayCharacter* Target, UBuffDataBase* base)
{
	if (!Target) return;
	if(Target->P_hp > 0)
	Target->P_hp = FMath::Clamp(Target->P_hp - base->Value,0.0f,Target->P_max_hp);
}