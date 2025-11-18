// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SavePoint.generated.h"

UCLASS()
class KITAKURPG_API ASavePoint : public AActor
{
	GENERATED_BODY()
	
private:
	FVector point;

public:	
	// Sets default values for this actor's properties
	ASavePoint();
	//ÉÅÉbÉVÉÖ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MESH")
	UStaticMeshComponent* BoxMesh;
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	class UBoxComponent* hit_collision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

};
