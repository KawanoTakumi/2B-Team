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


	//索敵範囲用スフィアの設定
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPHEAR")
	class USphereComponent* detectionSphere;

	//ジャンプできるかどうか（クールダウン判定）
	bool CanJumpToPlayer = false;
	

	//ジャンプのクールダウン時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JUMP")
	float jump_Cooldown = 2.0f;

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
	//敵の索敵範囲から外れた時
	UFUNCTION()
	void OnPlayerEnded(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	//ジャンプフラグを初期化
	void ResetJump();

private:
	FVector CurrentDirection = {0,0,0};//移動方向
	float ChangeDirectionInterval = 10.0f;//移動する方向を変えるまでの時間
	float TimeSinceLastChange = 0.0f;//最後に移動方向を変更した時間
	bool CanHitPlayer = false;//プレイヤーが当たったかどうか
	void ChooseNewDirection();
	class AMyPlayCharacter* CPlayer;//プレイヤーキャラクター
};