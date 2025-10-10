// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerChracterController.h"

AMyPlayerChracterController::AMyPlayerChracterController()
	:APlayerController()
	, m_pAMyCharacter(nullptr)
{

}

void AMyPlayerChracterController::BeginPlay()
{
	Super::BeginPlay();
	APawn* pPawn = GetPawn();
	ensure(pPawn != nullptr);
	m_pAMyCharacter = Cast<AMyPlayChracter>(pPawn);
	ensure(m_pAMyCharacter != nullptr);
}

void AMyPlayerChracterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerChracterController::MoveForward_Axis);
}

void AMyPlayerChracterController::MoveForward_Axis(float axisValue)
{
	check(m_pAMyCharacter != nullptr);
	m_pAMyCharacter->MoveForward(axisValue);
}

void AMyPlayerChracterController::MoveRight_Axis(float axisValue)
{
	check(m_pAMyCharacter != nullptr);
	m_pAMyCharacter->MoveRight(axisValue);
}