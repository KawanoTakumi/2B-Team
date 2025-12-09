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
#include "GameInstanceValue.h"
// Sets default values
AMyPlayCharacter::AMyPlayCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//スプリングアーム設定
	spring_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	spring_arm->SetupAttachment(RootComponent);
	spring_arm->TargetArmLength = 100.0f;
	spring_arm->bUsePawnControlRotation = true;
	spring_arm->bDoCollisionTest = true;
	spring_arm->bEnableCameraLag = true;
	spring_arm->CameraLagSpeed = 10.0f;
	spring_arm->ProbeChannel = ECC_Camera;
	spring_arm->ProbeSize = 12.0f;

	//カメラの設定
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComponent != nullptr);
	CameraComponent->SetupAttachment(spring_arm, USpringArmComponent::SocketName);
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f + BaseEyeHeight));
	CameraComponent->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void AMyPlayCharacter::BeginPlay()
{
	Super::BeginPlay();
	startPos = GetActorLocation();//最初の位置を取得
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayCharacter::OnCapsuleBeginOverlap);

	Status = this->FindComponentByClass<UStatusComponent>();
	//ステータス読み込み
	InputStatus();
	//HUD取得
	HUDwidget = Cast<AMyPlayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

// Called every frame
void AMyPlayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator ControlRot = GetControlRotation();


	// ピッチ角度を -20 〜 +20 に制限
	ControlRot.Pitch = FMath::ClampAngle(ControlRot.Pitch, -20.0f, 20.0f);

	// 制限した回転を適用
	GetController()->SetControlRotation(ControlRot);

	//ウィジェット更新
	if (HUDwidget)
	{
		HUDwidget->UpdateHPBar(g_player_hp, g_player_max_hp);
		HUDwidget->UpdateLevel(g_player_level);
	}

	//攻撃アニメーションの終了判定
	if (FA)
	{
		time++;
		if (time == 80)
		{
			time = 0;
			FA = false;
		}
	}

	//体力がなくなったら
	if (g_player_hp < 0)
	{
		g_player_hp = 50;
		//リスポーン地点に移動
		SetActorLocation(startPos);
	}
}


//ダメージ取得関数
float AMyPlayCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float GetDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	g_player_hp -= GetDamage;

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
	AddMovementInput(Direction, value * g_player_speed);
}
//左右移動
void AMyPlayCharacter::MoveRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value * g_player_speed);
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
		g_player_hp -= 10;
		SetActorLocation(startPos);
	}
	else if (OtherActor && OtherActor->ActorHasTag("Torch"))
	{
		//たいまつの場合
		torchCount++;
		OtherActor->Destroy();//拾ったら削除する
	}
	if (OtherActor && OtherActor->ActorHasTag("KEY"))
	{
		UE_LOG(LogTemp, Warning, TEXT("aaa"));
		//鍵の場合
		keyCount++;
		OtherActor->Destroy();
	}
}
//攻撃
void AMyPlayCharacter::Attack()
{
	if (!FA)
	{
		//攻撃アニメーションを再生
		if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
			FA = true;
		}

		//攻撃範囲の判定
		FVector Start = GetActorLocation();//検知開始場所
		FVector ForwardVector = CameraComponent->GetForwardVector();//検知終了場所
		FVector End = Start + ForwardVector * 30.0f; //200ユニット前方
		FVector Box_Scale = {85,85,85};
		FQuat rota = FQuat::Identity;

		TArray<FHitResult> hit_result;
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); //自分自身は無視

		//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start,
		//	End, ECC_Pawn, Params);

		bool bHit = GetWorld()->SweepMultiByChannel(hit_result, Start, End, rota, ECC_Pawn, FCollisionShape::MakeBox(Box_Scale),Params);

		if (bHit)
		{
			for (auto& Hit : hit_result)
			{
				// 壊れるBOXかどうか判定
				Abreakbox* HitBox = Cast<Abreakbox>(Hit.GetActor());
				if (HitBox)
				{
					HitBox->OnHitByPlayer(g_player_attack); // プレイヤーの攻撃力を渡す
				}
				else
				{
					//敵にダメージを与える
					UGameplayStatics::ApplyDamage(Hit.GetActor(), g_player_attack,
						GetController(), this, UDamageType::StaticClass());
				}

			}

		}
		// デバッグ表示 (箱のトレースを可視化)
		//DrawDebugBox(
		//	GetWorld(),
		//	End,
		//	Box_Scale,
		//	rota,
		//	FColor::Green,
		//	false,
		//	2.0f // 表示時間
		//);
	}
}
//ダメージ取得(TakeDamageの代替)
void AMyPlayCharacter::GetDamage(int damage)
{
	//攻撃力0あるいは体力0ならダメージ0として処理
	if (damage < 0 || g_player_hp < 0)return;
	UE_LOG(LogTemp, Warning, TEXT("Player HP : %f"), g_player_hp);
	g_player_hp -= damage;
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
	g_player_level++;
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
	OutputStatus();
}

void AMyPlayCharacter::AddBuff(UBuffDataBase* buffData)
{
	//nullチェック
	if (!buffData)return;

	//バフを適用
	UBuffEffectBase* Buff = NewObject<UBuffEffectBase>(this);
	Buff->ApplyEffect(this,buffData);
}

void AMyPlayCharacter::InputStatus()
{
	FString stage_name = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UGameInstanceValue* value = Cast<UGameInstanceValue>(GetWorld()->GetGameInstance());
	if (value == nullptr)return;

	if (stage_name == "Stage1_2")
	{
		if (Status)
		{
			value->Player_Attack = Status->Read_Attck;
			value->Player_HP = Status->Read_MAX_HP;
			value->Player_Speed = Status->Read_Speed;
			value->Player_level = 1;
		}
	}
	g_player_max_hp = value->Player_HP;;
	g_player_hp = g_player_max_hp;
    g_player_level = value->Player_level;
	g_player_attack = value->Player_Attack;
	g_player_speed = value->Player_Speed;
}

void AMyPlayCharacter::OutputStatus()
{
	UGameInstanceValue* value = Cast<UGameInstanceValue>(GetWorld()->GetGameInstance());

	value->Player_level = g_player_level;
	value->Player_Attack = g_player_attack;
	value->Player_Speed = g_player_speed;
}