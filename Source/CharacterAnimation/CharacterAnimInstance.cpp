// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "CharacterAnimationCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwningCharacter = Cast<ACharacterAnimationCharacter>(GetOwningActor());
	if (OwningCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("You are trying to use an anim instance for the wrong character."));
		return;
	}

	MovementComponent = Cast<UCharacterMovementComponent>(OwningCharacter->GetMovementComponent());
	if (MovementComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Movement component is not vaild."));
		return;
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (::IsValid(OwningCharacter) == false)
	{
		return;
	}

	GroundSpeed = FVector2D(MovementComponent->Velocity).Length();
	IsFailing = MovementComponent->IsFalling();

	/*
		Set Should Move to true only if ground speed is above a small threshold
		(to prevent incredibly small velocities from triggering animations)
		and if there is currently acceleration (input) applied.
	*/
	if (MovementComponent->GetCurrentAcceleration() != FVector(0.0f, 0.0f, 0.0f) && GroundSpeed > 0.3f)
	{
		ShouldMove = true;
	}
	else
	{
		ShouldMove = false;
	}
}
