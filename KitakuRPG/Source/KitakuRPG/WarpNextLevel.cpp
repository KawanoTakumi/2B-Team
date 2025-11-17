// Fill out your copyright notice in the Description page of Project Settings.


#include "WarpNextLevel.h"
#include "Components/StaticMeshComponent.h"//スタティックメッシュ作成に必要
#include "Components/BoxComponent.h"//球体作成に必要
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayCharacter.h"

// Sets default values
AWarpNextLevel::AWarpNextLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AWarpNextLevel::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AWarpNextLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarpNextLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//プレイヤー索敵関数
void AWarpNextLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<ACharacter>(OtherActor))
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			AMyPlayCharacter* TargetPlayer = Cast<AMyPlayCharacter>(OtherActor);
			if (TargetPlayer && TargetPlayer->keyCount > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("player hit"));
				//次のレベルに移動
				TargetPlayer->keyCount--;
				SetNextlevelName(level_name);
				
			}
		}
	}
}

void AWarpNextLevel::SetNextlevelName(const FString& name)
{
	UGameplayStatics::OpenLevel(this, FName(*name));
}