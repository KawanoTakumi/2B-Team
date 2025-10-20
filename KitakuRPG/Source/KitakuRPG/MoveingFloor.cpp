// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveingFloor.h"
// Sets default values
AMoveingFloor::AMoveingFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    RootComponent = PlatformMesh;

    MovementDirection = FVector(1.0f, 0.0f, 0.0f); // デフォルトはX方向
    MovementSpeed = 100.0f;
    MaxDistance = 500.0f;
    bMovingForward = true;

}

// Called when the game starts or when spawned
void AMoveingFloor::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
    TraveledDistance = 0.0f;
    // 動作範囲の終点を計算
    FVector EndLocation = StartLocation + MovementDirection.GetSafeNormal() * MaxDistance;

    // デバッグラインを表示（赤色、10秒間）
    DrawDebugLine(
        GetWorld(),
        StartLocation,
        EndLocation,
        FColor::Red,
        true, // 永続表示
        10.0f,
        0,
        5.0f // 太さ
    );

    // 終点にデバッグボックスを表示
    DrawDebugBox(
        GetWorld(),
        EndLocation,
        FVector(20.0f), // サイズ
        FColor::Green,
        true,
        10.0f
    );

}

// Called every frame
void AMoveingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FVector CurrentLocation = GetActorLocation();
    FVector DeltaMove = MovementDirection.GetSafeNormal() * MovementSpeed * DeltaTime;

    if (TraveledDistance >= MaxDistance)
    {
        bMovingForward = !bMovingForward;
        TraveledDistance = 0.0f;
    }

    FVector Move = bMovingForward ? DeltaMove : -DeltaMove;
    SetActorLocation(CurrentLocation + Move);
    TraveledDistance += Move.Size();
}