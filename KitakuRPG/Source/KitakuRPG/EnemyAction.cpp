// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAction.h"
#include "Components/SphereComponent.h"//���̃R���|�[�l���g
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include"MyPlayCharacter.h"

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
	bCanJumpToPlayer = true;
	detectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAction::OnPlayerDetected);
	ChooseNewDirection(); // ��������������

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: Overlap binding complete"));
}

// Called every frame
void AEnemyAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// �ړ�����
	AddMovementInput(CurrentDirection, 1.0f);

	// ��莞�Ԃ��Ƃɕ�����ύX
	TimeSinceLastChange += DeltaTime;
	if (TimeSinceLastChange >= ChangeDirectionInterval)
	{
		ChooseNewDirection();
		TimeSinceLastChange = 0.0f;
	}

}

// Called to bind functionality to input
void AEnemyAction::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAction::OnPlayerDetected(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

	if (!bCanJumpToPlayer)return;

	if (!OtherActor)return;
	UE_LOG(LogTemp, Warning, TEXT("HitPlayer"));

	//�v���C���[�擾
	AMyPlayCharacter* CPlayer = Cast<AMyPlayCharacter>(OtherActor);
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
	else
	{

		//�O�㍶�E�Ƀ����_���Ɉړ�����



		UE_LOG(LogTemp, Warning, TEXT("Not Get To Player!"));
	}

}

void AEnemyAction::ResetJump()
{
	bCanJumpToPlayer = true;
}


void AEnemyAction::ChooseNewDirection()
{
	// �O�㍶�E�̕����������_���ɑI��
	TArray<FVector> Directions = {
		GetActorForwardVector(),     // �O
		-GetActorForwardVector(),    // ��
		GetActorRightVector(),       // �E
		-GetActorRightVector()       // ��
	};

	int32 Index = FMath::RandRange(0, Directions.Num() - 1);
	CurrentDirection = Directions[Index];
}
