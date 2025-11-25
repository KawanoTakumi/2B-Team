// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyBullet.generated.h"

UCLASS()
class KITAKURPG_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBullet();
	//弾丸用のメッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MESH")
	UStaticMeshComponent* bulletMesh{};

	//あたり判定用スフィアの設定
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPHEAR")
	class USphereComponent* hitSphere{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SPHEAR")
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FireDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Attacked(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void GetAttackDamage();
};