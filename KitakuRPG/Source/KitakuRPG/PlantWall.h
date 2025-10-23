// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantWall.generated.h"

UCLASS()
class KITAKURPG_API APlantWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

	float break_timer = 10.0f;//壊れるまでの時間

	//壊れるまでの時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VALUE")
	float bleakTimer = 4.0f;

	//松明の消費数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	int torchCost = 1;
	//メッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MESH")
	UStaticMeshComponent* WallMesh;
	//球体メッシュ
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	class USphereComponent* TriggerSphere;

	
	//プレイヤー索敵、オブジェクト破壊関数
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
