// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MDPlayerController.h"

void AMDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly GameInputMode;
	SetInputMode(GameInputMode);
	SetShowMouseCursor(true);
}
