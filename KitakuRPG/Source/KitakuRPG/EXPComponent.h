// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EXPComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KITAKURPG_API UEXPComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEXPComponent();
	//経験値獲得
	void AddExp(int32 Amount);
	//レベルアップ
	void LevelUp();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 CurrentExp = 0;//取得経験値
	int32 CurrentLevel = 1;//レベル
	int32 ExpToNextLevel = 50;//次のレベルまでに必要な経験値
};