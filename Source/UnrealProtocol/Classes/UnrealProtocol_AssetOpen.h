// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealProtocol.h"

#include "UnrealProtocol_AssetOpen.generated.h"

namespace Unreal::Protocol
{
class FAssetOpen
{
	FAssetOpen() = default;
	friend struct FAssetOpenCallback;

	void Execute( const FString& RawURL );
	static constexpr FStringView Path = TEXTVIEW( "/asset/open" );

public:
	UNREALPROTOCOL_API static FString BuildLink( const FAssetData& AssetData );
};
}	 // namespace Unreal

UCLASS( BlueprintType )
class UNREALPROTOCOL_API UUnrealProtocol_AssetOpen : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
	void CopyLink();
};