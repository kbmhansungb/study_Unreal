// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterAnimationGameMode.h"
#include "CharacterAnimationCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACharacterAnimationGameMode::ACharacterAnimationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
