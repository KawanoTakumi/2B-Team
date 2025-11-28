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


}

// Called every frame
void AMoveingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    MoveToFloor(DeltaTime);
}

void AMoveingFloor::MoveToFloor(float Deltatime)
{
    if (!bCanMoved) return; // 待機中なら動かさない

    FVector CurrentLocation = GetActorLocation();
    FVector DeltaMove = MovementDirection.GetSafeNormal() * MovementSpeed * Deltatime;

    if (TraveledDistance >= MaxDistance)
    {
        bMovingForward = !bMovingForward;
        TraveledDistance = 0.0f;

        // ここで3秒待機開始
        bCanMoved = false;
        GetWorldTimerManager().SetTimer(RestartMovement, this, &AMoveingFloor::ResumeMove, 3.0f, false);
        return; // このTickでは動かさない
    }

    FVector Move = bMovingForward ? DeltaMove : -DeltaMove;
    SetActorLocation(CurrentLocation + Move);
    TraveledDistance += Move.Size();

}

void AMoveingFloor::ResumeMove()
{
    bCanMoved = true;
}