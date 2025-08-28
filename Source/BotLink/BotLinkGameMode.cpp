// Copyright Epic Games, Inc. All Rights Reserved.

#include "BotLinkGameMode.h"
#include "BotLinkCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABotLinkGameMode::ABotLinkGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
