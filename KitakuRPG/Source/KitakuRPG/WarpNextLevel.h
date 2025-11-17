// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarpNextLevel.generated.h"

UCLASS()
class KITAKURPG_API AWarpNextLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarpNextLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//メッシュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MESH")
	UStaticMeshComponent* WallMesh;
	//球体メッシュ
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "NAME")
	FString level_name;

	//プレイヤー索敵、オブジェクト破壊関数
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	
	void SetNextlevelName(const FString& name);

};