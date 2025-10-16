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
	bool bCanJumpToPlayer = false;

	//ジャンプのクールダウン時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JUMP")
	float jump_Cooldown = 3.0f;

	//ジャンプの強さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JUMP")
	float jump_Power = 100.0f;

	//ジャンプの高さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JUMP")
	float jump_Height = 100.0f;

	//関数//

	//敵がプレイヤーを見つけた時
	UFUNCTION()
	void OnPlayerDetected(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	void ResetJump();

private:
	FVector CurrentDirection;
	float ChangeDirectionInterval = 2.0f;
	float TimeSinceLastChange = 0.0f;

	void ChooseNewDirection();
	//ジャンプ解除


};
