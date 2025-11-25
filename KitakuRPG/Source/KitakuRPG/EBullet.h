// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EBullet.generated.h"

UCLASS()
class KITAKURPG_API AEBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//弾丸のメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MESH")
	UStaticMeshComponent* BulletMesh;

	//当たり判定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MESH")
	class USphereComponent* hitCollision;
	// 移動コンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void HitBullet(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 発射方向を設定する関数
	void FireInDirection(const FVector& ShootDirection);

};