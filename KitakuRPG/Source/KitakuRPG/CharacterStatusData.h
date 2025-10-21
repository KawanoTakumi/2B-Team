// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterStatusData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class KITAKURPG_API UCharacterStatusData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	//Å‘å‘Ì—Í
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "STATUS")
	int Character_MAX_HP = 0;
	//UŒ‚—Í
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "STATUS")
	int Character_Attack = 0;
	//ˆÚ“®‘¬“x
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "STATUS")
	float Character_Move_Speed = 1.0f;
	
};