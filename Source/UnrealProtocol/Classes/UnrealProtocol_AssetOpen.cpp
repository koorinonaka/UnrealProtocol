// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealProtocol_AssetOpen.h"

#include "EditorUtilityLibrary.h"
#include "HAL/PlatformApplicationMisc.h"

namespace UnrealProtocol
{
struct FAssetOpenCallback
{
	FAssetOpenCallback()
	{
		static FAssetOpen Singleton;
		UUnrealProtocol::SetCallback(
			FAssetOpen::Path.GetData(), UUnrealProtocol::FURIDelegate::CreateRaw( &Singleton, &FAssetOpen::Execute ) );
	}
} GAssetOpenCallback;

FString FAssetOpen::BuildLink( const FAssetData& AssetData )
{
	return UnrealProtocol::BuildLink( FString::Printf(
		TEXT( "%s?path=%s" ), Path.GetData(), *FGenericPlatformHttp::UrlEncode( AssetData.PackageName.ToString() ) ) );
}

void FAssetOpen::Execute( const FString& RawURL )
{
	TOptional<FString> PackageName = FGenericPlatformHttp::GetUrlParameter( RawURL, TEXT( "path" ) );
	if ( !PackageName.IsSet() )
	{
		UUnrealProtocol::ShowNotification(
			FString::Printf( TEXT( "[%s]:\nno specified path." ), Path.GetData() ), ENotification::Fail );
		return;
	}

	if ( GEditor )
	{
		auto* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
		check( AssetEditorSubsystem );

		AssetEditorSubsystem->OpenEditorForAsset( FGenericPlatformHttp::UrlDecode( PackageName.GetValue() ) );
	}
}
}	 // namespace UnrealProtocol

void UUnrealProtocol_AssetOpen::CopyLink()
{
	TArray<FString> ClipboardStrings;
	for ( const FAssetData& AssetData : UEditorUtilityLibrary::GetSelectedAssetData() )
	{
		ClipboardStrings.Emplace( UnrealProtocol::FAssetOpen::BuildLink( AssetData ) );
	}

	FPlatformApplicationMisc::ClipboardCopy( *FString::Join( ClipboardStrings, TEXT( "\n" ) ) );
}