// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayCharacter.generated.h"

UCLASS()
class KITAKURPG_API AMyPlayCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

protected:
	AMyPlayCharacter();

protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//ダメージ取得
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;


	//たいまつの所持数
	int torchCount = 0;

	//ステータス
	int P_attack;
	int P_max_hp;
	int P_hp;
	float P_speed;
	
	//初期位置
	FVector startPos;
	
	//ステータス読み込み
	class UStatusComponent* Status;

	UFUNCTION()
	void MoveForward(float value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	//マウスX軸回転
	UFUNCTION()
	void MTurn(float value);
	//マウスY軸回転
	UFUNCTION()
	void MLookUp(float value);
	//触れたアクタを取得
	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//攻撃
	UFUNCTION()
	void Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

	//
};