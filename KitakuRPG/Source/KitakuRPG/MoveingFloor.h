// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveingFloor.generated.h"

UCLASS()
class KITAKURPG_API AMoveingFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveingFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // スタティックメッシュ（ブループリントで設定可能）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    UStaticMeshComponent* PlatformMesh;

    // 移動方向（X/Y/Z）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    FVector MovementDirection;

    // 移動速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    float MovementSpeed;

    // 移動距離の最大値
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    float MaxDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
    float MaxmovementDeray = 50.0f;

    void MoveToFloor(float);
    void ResumeMove();
private:
    FVector StartLocation;
    FTimerHandle RestartMovement;
    float TraveledDistance;
    float moving_deray;
    bool bMovingForward;
    bool bCanMoved = true;
};