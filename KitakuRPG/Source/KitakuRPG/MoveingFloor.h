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

	//ÉÅÉbÉVÉÖ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* StaticMesh;
	//à⁄ìÆë¨ìx
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	float moveSpeed = 3.0f;
	//à⁄ìÆãóó£
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	FVector MovePos{200.0f,0.0f,0.0f};

	bool bMovingToTarget;
	FVector ActorPos;
	FVector NewPos;
};