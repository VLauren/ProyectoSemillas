// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SemillasGameMode.h"
#include "SemillasCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASemillasGameMode::ASemillasGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
