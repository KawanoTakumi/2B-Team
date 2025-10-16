// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerCharacterController.h"

AMyPlayerCharacterController::AMyPlayerCharacterController()
	:APlayerController()
	, m_pAMyCharacter(nullptr)
{

}

void AMyPlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	APawn* pPawn = GetPawn();
	ensure(pPawn != nullptr);
	m_pAMyCharacter = Cast<AMyPlayCharacter>(pPawn);
	ensure(m_pAMyCharacter != nullptr);
}

void AMyPlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AMyPlayerCharacterController::MoveForward_Axis);
	//InputComponent->BindAction("Jump",IE_Pressed, this, &AMyPlayerCharacterController::StartJump_Input);
	//InputComponent->BindAction("Jump",IE_Released, this, &AMyPlayerCharacterController::StopJump_Input);
}

void AMyPlayerCharacterController::MoveForward_Axis(float axisValue)
{
	check(m_pAMyCharacter != nullptr);
	m_pAMyCharacter->MoveForward(axisValue);
}

void AMyPlayerCharacterController::MoveRight_Axis(float axisValue)
{
	check(m_pAMyCharacter != nullptr);
	m_pAMyCharacter->MoveRight(axisValue);
}

//void AMyPlayerCharacterController::StartJump_Input()
//{
//	check(m_pAMyCharacter != nullptr);
//	m_pAMyCharacter->StartJump();
//}
//
//void AMyPlayerCharacterController::StopJump_Input()
//{
//	check(m_pAMyCharacter != nullptr);
//	m_pAMyCharacter->StopJump();
//}