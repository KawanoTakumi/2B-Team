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
	FString CurrentLevel = GetWorld()->GetCurrentLevel()->GetOutermost()->GetName();
	UGameInstanceValue* value = Cast<UGameInstanceValue>(GetWorld()->GetGameInstance());
	value->levelname = CurrentLevel;

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
		HUDwidget->UpdateHPBar(m_player_hp, m_player_max_hp);
		HUDwidget->UpdateLevel(m_player_level);
	}

	//攻撃アニメーションの終了判定
	if (m_attack_flag)
	{
		m_time++;
		//パーティクルを発生させる
		if (m_hit_enemy && m_time == 40)
		{
			if(particle)
				Hit_Effect();//エフェクト作成				
			if (Attack_1_se)
			UGameplayStatics::PlaySound2D(this, Attack_1_se);

		}

		if (m_time > 80)
		{

			m_time = 0;
			m_attack_flag = false;
		}
	}

	//体力がなくなったら
	if (m_player_hp < 0)
	{
		//一時的に最終チェックポイントに移動
		UGameplayStatics::OpenLevel(this, FName("GameOver"));
	}
}


//ダメージ取得関数
float AMyPlayCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float GetDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	m_player_hp -= GetDamage;

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
	AddMovementInput(Direction, value * m_player_speed);
}
//左右移動
void AMyPlayCharacter::MoveRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value * m_player_speed * 30);
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
		m_player_hp -= 10;
		SetActorLocation(startPos);
	}
	else if (OtherActor && OtherActor->ActorHasTag("Torch"))
	{
		//たいまつの場合
		torchCount++;
		Hit_Effect();
		OtherActor->Destroy();//拾ったら削除する
	}
	if (OtherActor && OtherActor->ActorHasTag("KEY"))
	{
		//鍵の場合
		keyCount++;
		OtherActor->Destroy();
	}
}
void AMyPlayCharacter::SearchAttackRange()
{
	//攻撃範囲の判定
	FVector Start = GetActorLocation();//検知開始場所
	FVector ForwardVector = CameraComponent->GetForwardVector();//検知終了場所
	FVector End = Start + ForwardVector * 130.0f;
	FVector Box_Scale = { 100,100,100 };//あたり判定の箱を設定
	FQuat rota = FQuat::Identity;

	TArray<FHitResult> hit_result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); //自分自身は無視


	bool bHit = GetWorld()->SweepMultiByChannel(hit_result, Start, End, rota, ECC_Pawn, FCollisionShape::MakeBox(Box_Scale), Params);
	if (bHit)
	{
		for (auto& Hit : hit_result)
		{
			UPrimitiveComponent* Hit_cmp = Hit.GetComponent();
			if (Hit_cmp && Hit_cmp->ComponentHasTag("Body"))
			{
				m_hit_enemy = true;

				//敵にダメージを与える
				UGameplayStatics::ApplyDamage(Hit.GetActor(), m_player_attack,
					GetController(), this, UDamageType::StaticClass());
			}
			// 壊れるBOXかどうか判定
			Abreakbox* HitBox = Cast<Abreakbox>(Hit.GetActor());
			if (HitBox)
			{
				HitBox->OnHitByPlayer(m_player_attack); // プレイヤーの攻撃力を渡す
			}
		}
	}

}

//攻撃
void AMyPlayCharacter::Attack()
{
	if (!m_attack_flag)
	{
		//攻撃アニメーションを再生
		if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
		{
			GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
			//攻撃サウンド再生
			if (m_hit_enemy)
			{
				//if (attack_se)
					//UGameplayStatics::PlaySound2D(this, attack_se);

				//Attack_1_se
			}

			m_attack_flag = true;
		}
		SearchAttackRange();//攻撃範囲を設定
	}
}
//ダメージ取得(TakeDamageの代替)
void AMyPlayCharacter::GetDamage(int damage)
{
	//攻撃力0あるいは体力0ならダメージ0として処理
	if (damage < 0 || m_player_hp < 0)return;
	UE_LOG(LogTemp, Warning, TEXT("Player HP : %f"), m_player_hp);
	m_player_hp -= damage;
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
	m_player_level++;
	P_EXP = 0;
	P_max_EXP = FMath::RoundToInt(P_max_EXP * 1.1f);//次のレベルまでの最大経験値量を指定
	if (LevelWidget)
	{
		//レベルアップ用のウィジェットを生成
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
	//初期ステージのみステータスデータを保存
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
	m_player_max_hp = value->Player_HP;;
	m_player_hp = m_player_max_hp;
    m_player_level = value->Player_level;
	m_player_attack = value->Player_Attack;
	m_player_speed = value->Player_Speed;
}

void AMyPlayCharacter::OutputStatus()
{
	UGameInstanceValue* value = Cast<UGameInstanceValue>(GetWorld()->GetGameInstance());

	value->Player_level = m_player_level;
	value->Player_Attack = m_player_attack;
	value->Player_Speed = m_player_speed;
}

//エフェクト関数
void AMyPlayCharacter::Hit_Effect()
{
	if (particle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			particle,
			GetActorLocation()+ GetActorForwardVector() * 150.0f,
			GetActorRotation()
		);
	}
	m_hit_enemy = false;
}