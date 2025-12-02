// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAction.h"
#include "Components/SphereComponent.h"//球体コンポーネント
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include"MyPlayCharacter.h"
#include "StatusComponent.h"//ステータスコンポーネント
#include "DrawDebugHelpers.h"
#include "EBullet.h"


// Sets default values
AEnemyAction::AEnemyAction()
{
	CurrentDirection = { 0,0,0 };
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//索敵用の球体を作成
	detectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	detectionSphere->SetupAttachment(RootComponent);
	detectionSphere->SetSphereRadius(500.0f);
	detectionSphere->SetCollisionProfileName(TEXT("Trigger"));

	//攻撃判定用の球体を作成
	attackedSphere = CreateDefaultSubobject<USphereComponent>("AttackedSphere");
	attackedSphere->SetupAttachment(RootComponent);
	attackedSphere->SetSphereRadius(100.0f);
	attackedSphere->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AEnemyAction::BeginPlay()
{
	Super::BeginPlay();
	SetStatus();
	CanJumpToPlayer = true;
	detectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAction::OnPlayerDetected);	
	attackedSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAction::Attacked);
	E_hp = max_hp;//最大体力に設定
	
	ChooseNewDirection(); // 初期方向を決定
}

// Called every frame
void AEnemyAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//攻撃のクールタイムを設定
	if (AttackTimer > 0 && !CanAttack)
	{
		AttackTimer--;
		if (AttackTimer <= 0)
		{
			CanAttack = true;
			AttackTimer = 60.0f* 2;//攻撃のタイマー


		}
			
	}

	TurnToPlayer();
	//プレイヤーが索敵範囲内に入れば
	if (CanHitPlayer)
	{
		//プレイヤーを発見した場合
		if (CPlayer)
		{
			//自身とプレイヤーの位置を設定
			FVector EnemyLoc = GetActorLocation();
			FVector PlayerLoc = CPlayer->GetActorLocation();
			FVector Direction = (PlayerLoc - EnemyLoc).GetSafeNormal();

			if (!CanJumpToPlayer)return;
			//取得したベクトルからジャンプする方向に発射
			FVector LaunchVelocity = (Direction)*jump_Power * 5;
			LaunchVelocity.Z = jump_Height * 3;
			LaunchCharacter(LaunchVelocity, true, true);

			CanJumpToPlayer = false;
			if (bullet)
			{
				FRotator SpawnRotation = GetActorRotation();
				FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 250.f;
				FActorSpawnParameters param;
				param.Owner = this;
				param.Instigator = GetInstigator();
				param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AEBullet* Bullet = GetWorld()->SpawnActor<AEBullet>(bullet.Get(), SpawnLocation, SpawnRotation, param);
				if (Bullet)
				{
					// 発射方向を回転からベクトルに変換
					FVector LaunchDirection = SpawnRotation.Vector();
					Bullet->FireInDirection(LaunchDirection);
					Bullet->m_damage = attack;
				}
			}

			//一定時間後に再びジャンプ可能にする
			GetWorldTimerManager().SetTimerForNextTick([this]()
				{
					FTimerHandle JumpTimerhandle;
					GetWorldTimerManager().SetTimer(JumpTimerhandle, this, &AEnemyAction::ResetJump, jump_Cooldown, false);
				});

		}
	}
	//前後左右にランダムに移動する
// 一定時間ごとに方向を変更
	if (!CanJumpToPlayer)
	{
		TimeSinceLastChange = DeltaTime;
		if (TimeSinceLastChange >= ChangeDirectionInterval)
		{
			ChooseNewDirection();
			TimeSinceLastChange = 0.0f;
		}
	}
	//移動している方向を前として回転させる
	FVector Velocity = GetVelocity();
	if (!Velocity.IsNearlyZero())
	{
		FRotator TargetRotation = Velocity.Rotation();
		TargetRotation.Pitch = 0.0f;
		TargetRotation.Roll = 0.0f;
		SetActorRotation(TargetRotation);
	}
}

void AEnemyAction::TurnToPlayer()
{
	//プレイヤーの方向に向かせる
	if (CanHitPlayer && CPlayer)
	{
		FVector Direction = (CPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator LookAtRotation = Direction.Rotation();
		LookAtRotation.Pitch = 0.0f;
		LookAtRotation.Roll = 0.0f;
		SetActorRotation(LookAtRotation);
	}

}

void AEnemyAction::SetStatus()
{
	//ステータスを読み込む
	EStatus = this->FindComponentByClass<UStatusComponent>();

	if (EStatus)
	{
		max_hp = EStatus->Read_MAX_HP;
		attack = EStatus->Read_Attck;
		speed = EStatus->Read_Speed;
		exp = EStatus->Read_EXP;
		AttackTimer = jump_Cooldown;
	}

}

//ダメージ取得関数
float AEnemyAction::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,AController* EventInstigator, AActor* DamageCauser)
{
	float GetDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	E_hp -= GetDamage;
	Hit_Effect();
	if (E_hp < 1)
	{
		//プレイヤーにEXPを付与
		if (CPlayer)
		{
			CPlayer->GetEXP(exp);
		}

		//ドロップアイテム
		DropItem();
		//オブジェクト削除
		this->Destroy();
	}
	return GetDamage;
}
// Called to bind functionality to input
void AEnemyAction::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//プレイヤー索敵
void AEnemyAction::OnPlayerDetected(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

	if (!OtherActor)return;
	// プレイヤー取得
	CPlayer = Cast<AMyPlayCharacter>(OtherActor);
	CanHitPlayer = true;
}
//プレイヤーが索敵範囲外にいった場合
void AEnemyAction::OnPlayerEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp)
{
	if (!OtherActor)return;
	CanHitPlayer = false;
	CPlayer = nullptr;
	hit = nullptr;//当たったアクターを初期化
}

void AEnemyAction::ResetJump()
{
	CanJumpToPlayer = true;
}

void AEnemyAction::DropItem()
{
	if (spawn_object)
	{
		FActorSpawnParameters param;
		param.Owner = this;

		GetWorld()->SpawnActor<AActor>(spawn_object, GetActorLocation(), GetActorRotation(), param);
	}
}

//攻撃
void AEnemyAction::Attacked(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	// プレイヤーかどうか判定
	if (OtherActor->ActorHasTag("Player")&& CanAttack == true)
	{
		hit = OtherActor;
		ActionInterval(hit);
	}
}
void AEnemyAction::ActionInterval(AActor* actor)
{
	if (!actor) return;
	CPlayer= Cast<AMyPlayCharacter>(actor);
	//プレイヤーが見つかっているかつ、ジャンプできる状態であればダメージを与える
	if (CPlayer && CanAttack && CanHitPlayer)
	{
		CanAttack = false;
		CPlayer->GetDamage(attack);
		FVector knockback = GetActorForwardVector() * 20.0f;
		knockback.Z = 30.0f;
		CPlayer->LaunchCharacter(knockback,true,true);
	}
}

void AEnemyAction::ChooseNewDirection()
{
	// 前後左右の方向をランダムに選択
	TArray<FVector> Directions = {
		GetActorForwardVector(),     // 前
		-GetActorForwardVector(),    // 後
		GetActorRightVector(),       // 右
		-GetActorRightVector()       // 左
	};

	int32 Index = FMath::RandRange(0, Directions.Num() - 1);
	CurrentDirection = Directions[Index];
}

void AEnemyAction::Hit_Effect()
{
	if (particle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			particle,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}