// Fill out your copyright notice in the Description page of Project Settings.


#include "UnsortedFunctionLibrary.h"

UFABRIKComponent::UFABRIKComponent() : DefaultTailVector(FVector::UnitX())
{
}

void UFABRIKComponent::AddSegment(const FFABRIKSegment& Segment)
{
	Segments.Add(Segment);

	Recalculated();

	//if (Segments.Num() > 1)
	//ApplyForwardFABRIK(Segments.Num() - 2, Segments.Num() - 1, Segments, Segments[Segments.Num() - 2].Position);
}

void UFABRIKComponent::InsertSegment(int32 Index, const FFABRIKSegment& Segment)
{
	if (Segments.IsValidIndex(Index) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Inserting a segment out of bounds."));
		return;
	}

	Segments.Insert(Segment, Index);

	Recalculated();
	//ApplyForwardFABRIK(Index, Segments.Num() - 1, Segments, Segments[Index].Position);
}

float UFABRIKComponent::GetLength(int32 Index)
{
	return Segments[Index].Length;
}

FVector UFABRIKComponent::GetPosition(int32 Index)
{
	return Segments[Index].Position;
}

void UFABRIKComponent::ApplyForward(const FVector& Target)
{
	ApplyForwardFABRIK(0, Segments.Num() - 1, Target);
}

void UFABRIKComponent::ApplyBackward(const FVector& Target)
{
	ApplyBackwardFABRIK(0, Segments.Num() - 1, Target);
}

void UFABRIKComponent::ApplyFixed(const FVector& Target)
{
	if (Segments.Num() == 0)
	{
		return;
	}

	const FVector FixedPosition = Segments.Last().Position;
	ApplyForwardFABRIK(0, Segments.Num() - 1, Target);
	ApplyBackwardFABRIK(0, Segments.Num() - 1, FixedPosition);
}

void UFABRIKComponent::ApplyWithIndex(int32 Index, const FVector& Target)
{
	ApplyForwardFABRIK(Index, Segments.Num() - 1, Target);
	ApplyBackwardFABRIK(0, Index, Target);
}

void UFABRIKComponent::Recalculated()
{
	ApplyForwardFABRIK(0, Segments.Num() - 1, Segments[0].Position);
}