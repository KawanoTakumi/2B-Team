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

	FTimerHandle HealTimer;


	UFUNCTION(BlueprintCallable)
	void ApplyEffect(AMyPlayCharacter* target,UBuffDataBase* bufftype);
	//ˆê•b–ˆ‚Ì‰ñ•œˆ—
	UFUNCTION()
	void HealTick(AMyPlayCharacter* Target);
	UFUNCTION()
	//ˆê•b‚²–ˆ‚Ìƒ_ƒ[ƒWˆ—
	void PoisonTick(AMyPlayCharacter* Target,UBuffDataBase* base);

};