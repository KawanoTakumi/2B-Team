// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayCharacter.h"
#include "Components/SphereComponent.h"//球体作成に必要
#include "GameFramework/Character.h"
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
	startPos = GetActorLocation();//最初の位置を取得
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayCharacter::OnCapsuleBeginOverlap);
}

// Called every frame
void AMyPlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator ControlRot = GetControlRotation();

	// ピッチ角度を -30 〜 +30 に制限
	ControlRot.Pitch = FMath::ClampAngle(ControlRot.Pitch, -10.0f, 10.0f);

	// 制限した回転を適用
	GetController()->SetControlRotation(ControlRot);

}

// Called to bind functionality to input
void AMyPlayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayCharacter::MoveRight);

	//マウスの視点移動バインド
	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayCharacter::MTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayCharacter::MLookUp);
	//PlayerInputComponent->BindAction("StartJump", IE_Pressed, this, &AMyPlayCharacter::StartJump);
	//PlayerInputComponent->BindAction("StopJump", IE_Released, this, &AMyPlayCharacter::StopJump);

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

//マウスX軸視点移動
void AMyPlayCharacter::MTurn(float value)
{
	AddControllerYawInput(value);
}
//マウスY軸視点移動
void AMyPlayCharacter::MLookUp(float value)
{
	AddControllerPitchInput(value);
}
//アクタを取得
void AMyPlayCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor->ActorHasTag("Water"))
	{
		//水の場合
		UE_LOG(LogTemp, Warning, TEXT("Waterに触れました！初期位置に戻します"));
		SetActorLocation(startPos);
	}
	else if (OtherActor && OtherActor->ActorHasTag("Torch"))
	{
		//たいまつの場合
		torchCount++;
		OtherActor->Destroy();//拾ったら削除する
	}
}
