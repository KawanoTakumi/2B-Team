// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveingFloor.h"
// Sets default values
AMoveingFloor::AMoveingFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMoveingFloor::BeginPlay()
{
	Super::BeginPlay();
    StaticMesh->S

	//ç≈èâÇÃà íuÇéÊìæ
    ActorPos = GetActorLocation();
	NewPos = ActorPos;
}

// Called every frame
void AMoveingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FVector CurrentLocation = GetActorLocation();
    FVector Destination = bMovingToTarget ? MovePos : ActorPos;

    FVector Direction = (Destination - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MovePos * DeltaTime;

    SetActorLocation(NewLocation);

    float DistanceToTarget = FVector::Dist(NewLocation, Destination);
    if (DistanceToTarget < 10.f)
    {
        bMovingToTarget = !bMovingToTarget;
    }
}