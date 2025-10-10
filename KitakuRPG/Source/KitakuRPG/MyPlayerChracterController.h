// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayChracter.h"
#include "MyPlayerChracterController.generated.h"

/**
 * 
 */
UCLASS()
class KITAKURPG_API AMyPlayerChracterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerChracterController();

protected:
	AMyPlayChracter* m_pAMyCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void MoveForward_Axis(float axisValue);
	void MoveRight_Axis(float axisValue);
};
