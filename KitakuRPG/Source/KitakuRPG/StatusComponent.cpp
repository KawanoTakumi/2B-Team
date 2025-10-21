// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusComponent.h"
#include "CharacterStatusData.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	if (StatusData)
	{
		//キャラクターのステータスデータを適用
		Read_MAX_HP = StatusData->Character_MAX_HP;
		Read_Attck = StatusData->Character_Attack;
		Read_Speed = StatusData->Character_Move_Speed;

	}

}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

