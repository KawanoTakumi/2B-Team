// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAction.generated.h"

UCLASS()
class KITAKURPG_API AEnemyAction : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ダメージ取得
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;


	//索敵範囲用スフィアの設定
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPHEAR")
	class USphereComponent* detectionSphere;

	//攻撃判定用スフィアの設定
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPHEAR")
	class USphereComponent* attackedSphere;

	//ジャンプできるかどうか（クールダウン判定）
	bool CanJumpToPlayer = false;
	

	//ジャンプのクールダウン時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JUMP")
	float jump_Cooldown = 15.0f;

	//ジャンプの強さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JUMP")
	float jump_Power = 500.0f;

	//ジャンプの高さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JUMP")
	float jump_Height = 400.0f;

	//関数//
public:
	//敵がプレイヤーを見つけた時行動
	UFUNCTION()
	void OnPlayerDetected(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPlayerEnded(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp);
	UFUNCTION()
	void Attacked(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	void ResetJump();
	//移動している方向を前として角度を回転させる
private:
	FVector CurrentDirection = {0,0,0};
	float ChangeDirectionInterval = 2.0f;
	float TimeSinceLastChange = 0.0f;
	bool CanHitPlayer = false;//プレイヤーが当たったかどうか
	void ChooseNewDirection();
	class AMyPlayCharacter* CPlayer;
	class UStatusComponent* EStatus;

	//ステータス（独立）
	int max_hp;
	int E_hp;
	int attack;
	float speed;
};