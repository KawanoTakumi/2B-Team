// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayCharacter.h"
#include "breakbox.h"
#include "Components/SphereComponent.h"//球体作成に必要
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "StatusComponent.h"
#include "MyPlayHUD.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpWidget.h"
#include "BuffDataBase.h"
#include "BuffEffectBase.h"

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
		P_hp = P_max_hp;
	}
	//HUD取得
	HUDwidget = Cast<AMyPlayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
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

	//ウィジェット更新
	if (HUDwidget)
	{
		HUDwidget->UpdateHPBar(P_hp, P_max_hp);
		HUDwidget->UpdateLevel(P_level);
	}


	//体力がなくなったら
	if (P_hp < 0)
	{
		P_hp = 50;
		//いったん初期地に戻す
		SetActorLocation(startPos);
	}
}

//ダメージ取得関数
float AMyPlayCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float GetDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	P_hp -= GetDamage;

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
//前後移動
void AMyPlayCharacter::MoveForward(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value * P_speed);
}
//左右移動
void AMyPlayCharacter::MoveRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value * P_speed);
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
//攻撃
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

		// 壊れるBOXかどうか判定
		Abreakbox* HitBox = Cast<Abreakbox>(HitResult.GetActor());
		if (HitBox)
		{
			HitBox->OnHitByPlayer(P_attack); // プレイヤーの攻撃力を渡す
		}
		else
		{
			//敵にダメージを与える
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), P_attack,
				GetController(), this, UDamageType::StaticClass());
		}
	}
}
//ダメージ取得(TakeDamageの代替)
void AMyPlayCharacter::GetDamage(int damage)
{
	//攻撃力0あるいは体力0なら破棄
	if (damage < 0 || P_hp < 0)return;
	P_hp -= damage;
}
//経験値獲得
void AMyPlayCharacter::GetEXP(int EXP)
{
	P_EXP += EXP;
	if (P_EXP > P_max_EXP)
		LevelUp();
}
//レベルアップ
void AMyPlayCharacter::LevelUp()
{
	P_level++;
	P_EXP = 0;
	P_max_EXP = FMath::RoundToInt(P_max_EXP * 1.2f);//次のレベルまでの最大経験値量を指定
	if (LevelWidget)
	{
		ULevelUpWidget* LevelUpWidget = CreateWidget<ULevelUpWidget>(GetWorld(), LevelWidget);

		if (LevelUpWidget)
		{
			LevelUpWidget->bIsFocusable = true;
			LevelUpWidget->AddToViewport();

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(LevelUpWidget->TakeWidget()); // ← フォーカス可能にする
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;

				UGameplayStatics::SetGamePaused(GetWorld(), true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not get widget"));
		}
	}
}

void AMyPlayCharacter::AddBuff(UBuffDataBase* buffData)
{
	//nullチェック
	if (!buffData)return;

	//バフを適用
	UBuffEffectBase* Buff = NewObject<UBuffEffectBase>(this);
	Buff->ApplyEffect(this,buffData);
}