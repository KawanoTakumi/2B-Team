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
		Target->P_attack += buffType->Value;
		break;
	case EBuffType::Heal:
		Target->P_hp = FMath::Clamp(Target->P_hp + buffType->Value, 0.0f, Target->P_max_hp);
		break;

	case EBuffType::SpeedUp:
		Target->P_speed = buffType->Value;
		break;
	default:
		break;
	}
}