// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "CharacterAnimation.h"
#include "CharacterAnimationCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwningCharacter = Cast<ACharacterAnimationCharacter>(GetOwningActor());
	if (OwningCharacter == nullptr)
	{
		UE_LOG(CharacterAnimation, Error, TEXT("You are trying to use an anim instance for the wrong character."));
		return;
	}

	MovementComponent = Cast<UCharacterMovementComponent>(OwningCharacter->GetMovementComponent());
	if (MovementComponent == nullptr)
	{
		UE_LOG(CharacterAnimation, Error, TEXT("Movement component is not vaild."));
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
		지면 속도가 작은 임계값보다 높은 경우에만 이동해야 함을 true로 설정합니다.
		(매우 작은 속도가 애니메이션을 트리거하는 것을 방지하기 위해)
		현재 가속(입력)이 적용된 경우.
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
