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

namespace Unreal::Protocol
{
class FMapJump
{
	FMapJump() = default;
	friend struct FMapJumpCallback;

	void Execute( const FString& RawURL );
	static constexpr FStringView Path = TEXTVIEW( "/map/jump" );

public:
	UNREALPROTOCOL_API static FString BuildLink( const FUnrealProtocol_MapJumpContext& Context );
};
}	 // namespace Unreal

UCLASS( BlueprintType )
class UNREALPROTOCOL_API UUnrealProtocol_MapJump : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	void CopyLink();
};