// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterAnimationGameMode.h"
#include "CharacterAnimationCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACharacterAnimationGameMode::ACharacterAnimationGameMode()
{
	DefaultPawnClass = ACharacterAnimationCharacter::StaticClass();
}
