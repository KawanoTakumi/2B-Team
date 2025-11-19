// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
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
public:
	class UStatusComponent* EStatus;


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

	//パーティクル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* particle;
	//スポーンさせたいオブジェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
	TSubclassOf<AActor> spawn_object;

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

	void SetStatus();
	void TurnToPlayer();
	void ActionInterval(AActor* actor);
	void DropItem();
	//エフェクト発生関数
	UFUNCTION()
	void Hit_Effect();

	void ResetJump();
	//移動している方向を前として角度を回転させる
private:
	FVector CurrentDirection = {0,0,0};
	AActor* hit;
	float ChangeDirectionInterval = 2.0f;
	float TimeSinceLastChange = 0.0f;
	bool CanHitPlayer = false;//プレイヤーが当たったかどうか
	void ChooseNewDirection();
	class AMyPlayCharacter* CPlayer;
	bool CanAttack = false;//攻撃できるかどうか
	float AttackTimer = 0.0f;//攻撃までの時間
	//ステータス（独立）
	int max_hp;//最大体力
	int E_hp;//体力
	int attack;//攻撃力
	float speed;//移動速度
	int exp;//経験値
};