// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameModeBase.h"
#include "TitleHUD.h"

ATitleGameModeBase::ATitleGameModeBase()
{
	HUDClass = ATitleHUD::StaticClass();
}
