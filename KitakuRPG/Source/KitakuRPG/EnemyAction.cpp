// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAction.h"
#include "Components/SphereComponent.h"//球体コンポーネント
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include"MyPlayCharacter.h"


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

}

// Called when the game starts or when spawned
void AEnemyAction::BeginPlay()
{
	Super::BeginPlay();
	CanJumpToPlayer = true;
	detectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAction::OnPlayerDetected);	

	ChooseNewDirection(); // 初期方向を決定
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: Overlap binding complete"));
}

// Called every frame
void AEnemyAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanHitPlayer)
	{
		if (!CPlayer)
		{
			//nullだった場合
			UE_LOG(LogTemp, Warning, TEXT("プレイヤーが見つかりません！！"));
			return;
		}
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

			//一定時間後に再びジャンプ可能にする
			GetWorldTimerManager().SetTimerForNextTick([this]()
				{
					FTimerHandle JumpTimerhandle;
					GetWorldTimerManager().SetTimer(JumpTimerhandle, this, &AEnemyAction::ResetJump, jump_Cooldown, false);
				});
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not Search Player"));
		}

	}
	else
	{
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

}

// Called to bind functionality to input
void AEnemyAction::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAction::OnPlayerDetected(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

	if (!OtherActor)return;
	//プレイヤー取得
	CPlayer = Cast<AMyPlayCharacter>(OtherActor);
	CanHitPlayer = true;

}
void AEnemyAction::OnPlayerEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp)
{
	if (!OtherActor)return;
	CanHitPlayer = false;
}

void AEnemyAction::ResetJump()
{
	CanJumpToPlayer = true;
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
