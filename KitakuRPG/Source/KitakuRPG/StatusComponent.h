// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KITAKURPG_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STATUS")
	class UCharacterStatusData* StatusData;


	//ì«Ç›çûÇ‹ÇÍÇΩç≈ëÂëÃóÕ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE")
	int Read_MAX_HP;

	//ì«Ç›çûÇ‹ÇÍÇΩçUåÇóÕ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE")
	int Read_Attck;

	//ì«Ç›çûÇ‹ÇÍÇΩà⁄ìÆë¨ìx
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VALUE")
	float Read_Speed;
};
