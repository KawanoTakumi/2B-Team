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
	UFUNCTION()
	void HealTick(AMyPlayCharacter* Target);
};