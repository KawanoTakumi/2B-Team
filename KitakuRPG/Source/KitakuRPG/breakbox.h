// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "breakbox.generated.h"

UCLASS()
class KITAKURPG_API Abreakbox : public AActor
{
	GENERATED_BODY()
	
public:	
	Abreakbox();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoxMesh;

	UFUNCTION()
	void OnHitByPlayer(float Damage);

	// HPÇéùÇΩÇπÇÈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box")
	float BoxHP = 1.0f;

};
