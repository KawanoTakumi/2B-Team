// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAction.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "SpawnActorObj.generated.h"

UCLASS()
class KITAKURPG_API ASpawnActorObj : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnActorObj();
	//スポーンさせるアクター
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Acotr")
	TSubclassOf<AEnemyAction> spawn_object;	
	//再スポーンまでにかかる時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	float spawn_interval = 300.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	class USphereComponent* hit_collision;
	//パーティクル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* particle = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnActor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	bool m_can_spawn_actor = true;
	float m_spawn_timer = spawn_interval;
};
