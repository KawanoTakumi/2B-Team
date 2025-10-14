// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayCharacter.h"

// Sets default values
AMyPlayCharacter::AMyPlayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComponent != nullptr);

	CameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	CameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMyPlayCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayCharacter::MoveRight);

	PlayerInputComponent->BindAction("StartJump", IE_Pressed, this, &AMyPlayCharacter::StartJump);
	PlayerInputComponent->BindAction("StopJump", IE_Released, this, &AMyPlayCharacter::StopJump);

}

void AMyPlayCharacter::MoveForward(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void AMyPlayCharacter::MoveRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void AMyPlayCharacter::StartJump()
{
	bPressedJump = true;
}

void AMyPlayCharacter::StopJump()
{
	bPressedJump = false;
}