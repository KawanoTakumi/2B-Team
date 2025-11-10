// Fill out your copyright notice in the Description page of Project Settings.


#include "breakbox.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
// Sets default values
Abreakbox::Abreakbox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("BoxMesh"));
	RootComponent = BoxMesh;


	// メッシュを設定（プロジェクトにインポート済みのものを使う）
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Meshes/WoodenBox.WoodenBox"));
	if (MeshAsset.Succeeded())
	{
		BoxMesh->SetStaticMesh(MeshAsset.Object);
	}

	// コリジョン設定（攻撃判定用）
	BoxMesh->SetCollisionProfileName(TEXT("BlockAll"));

}

// Called when the game starts or when spawned
void Abreakbox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Abreakbox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Abreakbox::OnHitByPlayer(float Damage)
{
	BoxHP -= Damage;
	SpawnEffect();
	if (BoxHP <= 0)
	{
		Destroy();
	}
}

void Abreakbox::SpawnEffect()
{
	if (n_effect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			n_effect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}