// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceValue.h"
void UGameInstanceValue::Init()
{
	Super::Init();

	Player_HP = 100;
	Player_Attack = 1;
	Player_level = 1;
	Player_Speed = 1.0f;
}