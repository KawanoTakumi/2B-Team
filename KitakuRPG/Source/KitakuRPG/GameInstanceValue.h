// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceValue.generated.h"

/**
 * 
 */
UCLASS()
class KITAKURPG_API UGameInstanceValue : public UGameInstance
{
	GENERATED_BODY()
public:

	virtual void Init() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
	float Player_HP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
	int Player_level;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
	int Player_Attack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData")
	float Player_Speed;

};