// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "JsonObjectConverter.h"
#include "Modules/ModuleManager.h"

#include "UnrealProtocol.generated.h"

class FUnrealProtocolModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

UENUM()
enum class ENotification : uint8
{
	None,
	Pending,
	Success,
	Fail,
};

UCLASS()
class UNREALPROTOCOL_API UUnrealProtocol : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam( FURIDelegate, const FString& RawURL );
	static void SetCallback( const FName Path, const FURIDelegate& Callback );

	UFUNCTION( BlueprintCallable )
	static void Entry( const FString& URL );

	UFUNCTION( BlueprintCallable, meta = ( AdvancedDisplay = "ExpireDuration" ) )
	static bool ShowNotification( const FText Text, ENotification CompletionState, const float ExpireDuration = 4.0f );
	static bool ShowNotification( const FString Message, ENotification CompletionState, const float ExpireDuration = 4.0f );
};

namespace UnrealProtocol
{
FString BuildLink( const FString& URL );

template <typename T>
bool Serialize( const T& Source, FString& Destination )
{
	return FJsonObjectConverter::UStructToJsonObjectString<T>( Source, Destination, 0, 0, 0, nullptr, false );
}

template <typename T>
bool Deserialize( const FString& Source, T& Destination )
{
	return FJsonObjectConverter::JsonObjectStringToUStruct<T>( Source, &Destination );
}
}