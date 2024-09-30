// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealProtocol.h"
#include "UnrealProtocol_AssetOpen.generated.h"

namespace UnrealProtocol
{
class FAssetOpen
{
public:
	static constexpr FStringView Path = TEXTVIEW( "/asset/open" );
	UNREALPROTOCOL_API static FString BuildLink( const FAssetData& AssetData );
};
}	 // namespace UnrealProtocol

UCLASS( BlueprintType )
class UNREALPROTOCOL_API UUnrealProtocol_AssetOpen : public UObject
{
	GENERATED_BODY()

public:
	UUnrealProtocol_AssetOpen();

	UFUNCTION( BlueprintCallable )
	void CopyLink();
};
