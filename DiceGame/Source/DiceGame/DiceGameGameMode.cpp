// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "DiceGame.h"
#include "DiceGameGameMode.h"
#include "DiceGameHUD.h"
#include "DiceGameCharacter.h"

ADiceGameGameMode::ADiceGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ADiceGameHUD::StaticClass();
}
