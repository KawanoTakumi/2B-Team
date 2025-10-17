// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayCharacter.generated.h"

UCLASS()
class KITAKURPG_API AMyPlayCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

protected:
	AMyPlayCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	//ƒ}ƒEƒXXŽ²‰ñ“]
	UFUNCTION()
	void MTurn(float value);
	UFUNCTION()
	void MLookUp(float value);
};
