// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuffDataBase.generated.h"

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	AttackUp,
	Heal,
	SpeedUp,
};

/**
 * 
 */
UCLASS(BlueprintType)
class KITAKURPG_API UBuffDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	//バフの名称
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName BuffName;

	//バフの強さ
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Value;

	//バフタイプ
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EBuffType BuffType;

};