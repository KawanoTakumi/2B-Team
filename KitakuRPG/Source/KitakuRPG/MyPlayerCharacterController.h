// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayCharacter.h"
#include "MyPlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class KITAKURPG_API AMyPlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerCharacterController();

protected:
	AMyPlayCharacter* m_pAMyCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void MoveForward_Axis(float axisValue);
	void MoveRight_Axis(float axisValue);

	void StartJump_Input();
	void StopJump_Input();
};
