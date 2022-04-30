// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UnsortedFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct CHARACTERANIMATION_API FFABRIKSegment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Position;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
		float Length;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class CHARACTERANIMATION_API UFABRIKComponent final : public UActorComponent
{
	GENERATED_BODY()

public:
	UFABRIKComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultTailVector;

public:
	UFUNCTION(BlueprintCallable)
	void AddSegment(const FFABRIKSegment& Segment);

	UFUNCTION(BlueprintCallable)
	void InsertSegment(int32 Index, const FFABRIKSegment& Segment);

	UFUNCTION(BlueprintCallable)
	float GetLength(int32 Index);

	UFUNCTION(BlueprintCallable)
	FVector GetPosition(int32 Index);

	/*
	*	Makes the first head reach the desired position.
	*/
	UFUNCTION(BlueprintCallable)
	void ApplyForward(const FVector& Target);

	/*
	*	Makes the last tail reach the desired position.
	*/
	UFUNCTION(BlueprintCallable)
	void ApplyBackward(const FVector& Target);

	/*
	*	The FABRIK algorithm in which the position of the tail is fixed is applied.
	*/
	UFUNCTION(BlueprintCallable)
	void ApplyFixed(const FVector& Target);

	/*
	*	Allows the segment at a specific index to reach the desired position.
	*/
	UFUNCTION(BlueprintCallable)
	void ApplyWithIndex(int32 Index, const FVector& Target);

	UFUNCTION(BlueprintCallable)
	void Recalculated();

private:
	/*
	*	The first index means the first head.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<FFABRIKSegment> Segments;

private:
	FORCEINLINE bool IsVaildIndex(int Start, int End)
	{
		bool IsVaild = true;
		if (Segments.IsValidIndex(Start) == false)
		{
			IsVaild = false;
		}

		if (Segments.IsValidIndex(End) == false)
		{
			IsVaild = false;
		}

		if (IsVaild == false)
		{
			UE_LOG(LogTemp, Error,
				TEXT("Invalid index use. Start : %d, End : %d, SegmentsNum : %d"),
				Start, End, Segments.Num());
		}
		return IsVaild;
	}

	FORCEINLINE FVector GetNewTailPosition(float Length, const FVector& Tail, const FVector& Target)
	{
		FVector TargetToTailVector = Tail - Target;
		const float SquareSum = TargetToTailVector.SquaredLength();

		// There is a problem that the position of
		//the tail becomes NAN when Square Sum is 0.
		if (SquareSum < FLT_EPSILON)
		{
			return Target + DefaultTailVector * Length;
		}
		else
		{
			TargetToTailVector *= FMath::InvSqrt(SquareSum);
			return Target + TargetToTailVector * Length;
		}
	}

	FORCEINLINE void UpdateTailPosition(float Length, FVector& Tail, const FVector& Target)
	{
		Tail = GetNewTailPosition(Length, Tail, Target);
	}

	FORCEINLINE void ApplyForwardFABRIK(int Start, int End, FVector Target)
	{
		if (IsVaildIndex(Start, End) == false)
		{
			return;
		}

		while (Start < End)
		{
			Segments[Start].Position = Target;
			UpdateTailPosition(Segments[Start + 1].Length, Segments[Start + 1].Position, Target);
			Target = Segments[Start + 1].Position;
			Start++;
		}
	}

	FORCEINLINE void ApplyBackwardFABRIK(int Start, int End, FVector Target)
	{
		if (IsVaildIndex(Start, End) == false)
		{
			return;
		}

		while (Start < End)
		{
			Segments[End].Position = Target;
			UpdateTailPosition(Segments[End].Length, Segments[End - 1].Position, Target);
			Target = Segments[End - 1].Position;
			End--;
		}
	}
};