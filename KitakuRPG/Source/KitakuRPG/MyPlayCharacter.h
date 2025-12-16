// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyPlayHUD.h"
#include "MyPlayCharacter.generated.h"


UCLASS()
class KITAKURPG_API AMyPlayCharacter : public ACharacter
{
	GENERATED_BODY()



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

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* spring_arm;



	//たいまつの所持数
	UPROPERTY()
	int torchCount = 0;
	//鍵の所持数
	UPROPERTY()
	int keyCount = 0;
	//ステータス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "STATUS")
	int m_player_attack = 1;
	int m_player_max_hp = 1;
	int m_player_level = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "STATUS")
	float m_player_hp = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "STATUS")
	float m_player_speed = 1;
	float m_heal_by_time = 0;//自然回復力
	int P_EXP = 1;
	int P_max_EXP = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FLAG")
	bool m_attack_flag = false;
	bool m_hit_enemy = false;
	int m_time = 0;
	//初期位置
	FVector startPos;
	
	//ステータス読み込み
	class UStatusComponent* Status;
	void InputStatus();
	void OutputStatus();

	UFUNCTION()
	void MoveForward(float value);//前後移動

	UFUNCTION()
	void MoveRight(float Value);//左右移動

	UFUNCTION()
	void StartJump();//ジャンプ開始

	UFUNCTION()
	void StopJump();//ジャンプ終了

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
	void SearchAttackRange();
	//ダメージ取得
	UFUNCTION()
	void GetDamage(int damage);

	//経験値獲得
	UFUNCTION()
	void GetEXP(int CurrentEXP);

	//レベルアップ
	UFUNCTION()
	void LevelUp();
	//エフェクト発生関数
	UFUNCTION()
	void Hit_Effect();

	//バフ適用
	UFUNCTION()
	void AddBuff(class UBuffDataBase* BuffData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

	AMyPlayHUD* HUDwidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget>LevelWidget;
	//パーティクル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* particle;

};