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
		Target->g_player_attack += buffType->Value;
	}break;
	case EBuffType::Heal:
	{
		// 1秒ごとに HealTick を呼び出す
		if (Target)
		{
			Target->g_heal_by_time += buffType->Value;//自然回復力を強化
			FTimerDelegate HealDelegate;
			HealDelegate.BindUFunction(this, FName("HealTick"), Target);
			GetWorld()->GetTimerManager().SetTimer(HealTimer, HealDelegate, 1.0f, true);
		}
	} break;
	case EBuffType::SpeedUp:
	{
		//スピードアップ
		Target->g_player_speed += buffType->Value;
	}break;
	case EBuffType::SpeedDown:
	{
		//スピードダウン
		Target->g_player_speed -= buffType->Value;

		FTimerDelegate SPEED_stop_delegate;
		SPEED_stop_delegate.BindUFunction(this, FName("SPEED_downTick"), Target);
		GetWorld()->GetTimerManager().SetTimer(SPEED_down_Timer, SPEED_stop_delegate, buffType->Timer, false);

	}break;
	case EBuffType::AttackDown:
	{
		//攻撃力ダウン
		if (Target)
		{
			Target->g_player_attack -= buffType->Value;
			
			FTimerDelegate ATK_stop_delegate;
			ATK_stop_delegate.BindUFunction(this, FName("ATK_downTick"), Target);
			GetWorld()->GetTimerManager().SetTimer(ATK_down_Timer, ATK_stop_delegate, buffType->Timer, false);
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
			GetWorld()->GetTimerManager().SetTimer(PoisonTimer, PoisonDelegate, buffType->Timer/5, true);//5回ダメージを与える


			//毒終了デリゲート
			FTimerDelegate PoisonStopDelegate;
			PoisonStopDelegate.BindUFunction(this, FName("StopPoisonTimer"), Target);
			GetWorld()->GetTimerManager().SetTimer(PoisonTimer, PoisonStopDelegate,buffType->Timer, false);
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

	Target->g_player_hp = FMath::Clamp(Target->g_player_hp + Target->g_heal_by_time, 0.0f, Target->g_player_max_hp);
}
//毒処理
void UBuffEffectBase::PoisonTick(AMyPlayCharacter* Target, UBuffDataBase* base)
{
	if (!Target) return;
	if(Target->g_player_hp > 0)
	Target->g_player_hp = FMath::Clamp(Target->g_player_hp - base->Value,0.0f,Target->g_player_max_hp);
}
//攻撃ダウン処理
void UBuffEffectBase::ATK_downTick(AMyPlayCharacter* Target, UBuffDataBase* base)
{
	GetWorld()->GetTimerManager().ClearTimer(ATK_down_Timer);
	Target->g_player_attack += base->Value;
}
//スピードダウン処理
void UBuffEffectBase::SPEED_downTick(AMyPlayCharacter* Target, UBuffDataBase* base)
{
	GetWorld()->GetTimerManager().ClearTimer(SPEED_down_Timer);
	Target->g_player_speed += base->Value;
}
void UBuffEffectBase::StopPoisonTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(PoisonTimer);
}