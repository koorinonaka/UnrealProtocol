// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UnrealProtocol.h"
#include "UnrealProtocol_MapJump.generated.h"

USTRUCT( BlueprintType )
struct FUnrealProtocol_MapJumpContext
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSoftObjectPtr<UWorld> World;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FVector CameraLocation = FVector::ZeroVector;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FRotator CameraRotation = FRotator::ZeroRotator;
};

namespace UnrealProtocol
{
class FMapJump
{
public:
	static constexpr FStringView Path = TEXTVIEW( "/map/jump" );
	UNREALPROTOCOL_API static FString BuildLink( const FUnrealProtocol_MapJumpContext& Context );
};
}	 // namespace UnrealProtocol

UCLASS( BlueprintType )
class UNREALPROTOCOL_API UUnrealProtocol_MapJump : public UObject
{
	GENERATED_BODY()

public:
	UUnrealProtocol_MapJump();

	UFUNCTION( BlueprintCallable )
	void CopyLink();
};
