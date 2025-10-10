// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAction.h"
#include "Components/SphereComponent.h"//���̃R���|�[�l���g
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AEnemyAction::AEnemyAction()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//���G�p�̋��̂��쐬
	detectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	detectionSphere->SetupAttachment(RootComponent);
	detectionSphere->SetSphereRadius(500.0f);
	detectionSphere->SetCollisionProfileName(TEXT("Trigger"));

}

// Called when the game starts or when spawned
void AEnemyAction::BeginPlay()
{
	Super::BeginPlay();
	detectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAction::OnPlayerDectected);
}

// Called every frame
void AEnemyAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyAction::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAction::OnPlayerDectected(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanJumpToPlayer)return;

	if (!OtherActor)return;

	//�v���C���[�擾
	ACharacter* CPlayer = Cast<ACharacter>(OtherActor);
	//�v���C���[�𔭌������ꍇ
	if (CPlayer)
	{
		//���g�ƃv���C���[�̈ʒu��ݒ�
		FVector EnemyLoc = GetActorLocation();
		FVector PlayerLoc = CPlayer->GetActorLocation();
		FVector Direction = (EnemyLoc - PlayerLoc).GetSafeNormal();

		//�擾�����x�N�g������W�����v��������ɔ���
		FVector LaunchVelocity = Direction * jump_Power;
		LaunchVelocity.Z = jump_Height;
		LaunchCharacter(LaunchVelocity, true, true);

		bCanJumpToPlayer = false;
		
		//��莞�Ԍ�ɍĂуW�����v�\�ɂ���
		GetWorldTimerManager().SetTimerForNextTick([this]()
		{
				FTimerHandle JumpTimerhandle;
				GetWorldTimerManager().SetTimer(JumpTimerhandle, this, &AEnemyAction::ResetJump, jump_Cooldown, false);
		});

	}
}

void AEnemyAction::ResetJump()
{
	bCanJumpToPlayer = true;
}