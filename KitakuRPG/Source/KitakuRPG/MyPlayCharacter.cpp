// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayCharacter.h"
#include "Components/SphereComponent.h"//球体作成に必要
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "StatusComponent.h"
#include "MyPlayHUD.h"
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

	Status = this->FindComponentByClass<UStatusComponent>();
	//ステータス読み込み
	if (Status)
	{
		P_attack = Status->Read_Attck;
		P_max_hp = Status->Read_MAX_HP;
		P_speed = Status->Read_Speed;
	}
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



	//HPバーを更新
	if(HUDwidget)
		HUDwidget->UpdateHPBar(P_hp,P_max_hp);
}

//ダメージ取得関数
float AMyPlayCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float GetDamage = DamageAmount;
	P_hp -= GetDamage;
	UE_LOG(LogTemp, Warning, TEXT("Player_HP"));
	if (P_hp < 1)
	{
		P_hp = 1;
		//いったん初期地に戻す
		SetActorLocation(startPos);
	}
	//HPバーを更新
	if (HUDwidget)
		HUDwidget->UpdateHPBar(P_hp, P_max_hp);

	return GetDamage;
}

// Called to bind functionality to input
void AMyPlayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//プレイヤー移動
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayCharacter::MoveRight);

	//マウスの視点移動バインド
	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayCharacter::MTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayCharacter::MLookUp);

	//プレイヤー攻撃
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPlayCharacter::Attack);
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
		SetActorLocation(startPos);
	}
	else if (OtherActor && OtherActor->ActorHasTag("Torch"))
	{
		//たいまつの場合
		torchCount++;
		OtherActor->Destroy();//拾ったら削除する
	}
}

void AMyPlayCharacter::Attack()
{
	//攻撃アニメーションを再生
	if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}

	//攻撃範囲の判定
	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = Start + ForwardVector * 200.0f; //200ユニット前方

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); //自分自身は無視

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start,
		End, ECC_Pawn, Params);

	if (bHit && HitResult.GetActor())
	{
		//敵にダメージを与える
		UGameplayStatics::ApplyDamage(HitResult.GetActor(),P_attack,
			GetController(), this, UDamageType::StaticClass());
	}
}