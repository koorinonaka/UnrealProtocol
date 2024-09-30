// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealProtocol_AssetOpen.h"

#include "EditorUtilityLibrary.h"
#include "HAL/PlatformApplicationMisc.h"

namespace UnrealProtocol
{
FString FAssetOpen::BuildLink( const FAssetData& AssetData )
{
	return UnrealProtocol::BuildLink( FString::Printf(
		TEXT( "%s?path=%s" ), Path.GetData(), *FGenericPlatformHttp::UrlEncode( AssetData.PackageName.ToString() ) ) );
}
}	 // namespace UnrealProtocol

using namespace UnrealProtocol;

UUnrealProtocol_AssetOpen::UUnrealProtocol_AssetOpen()
{
	struct FLocal
	{
		static void Execute( const FString& RawURL )
		{
			TOptional<FString> PackageName = FGenericPlatformHttp::GetUrlParameter( RawURL, TEXT( "path" ) );
			if ( !PackageName.IsSet() )
			{
				UUnrealProtocol::ShowNotification(
					FString::Printf( TEXT( "[%s]:\nno specified path." ), FAssetOpen::Path.GetData() ), ENotification::Fail );
				return;
			}

			if ( GEditor )
			{
				auto* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
				check( AssetEditorSubsystem );

				AssetEditorSubsystem->OpenEditorForAsset( FGenericPlatformHttp::UrlDecode( PackageName.GetValue() ) );
			}
		}
	};

	if ( HasAnyFlags( RF_ClassDefaultObject ) )
	{
		GetMutableDefault<UUnrealProtocol>()->Delegates.FindOrAdd(
			FAssetOpen::Path.GetData(), UUnrealProtocol::FURIDelegate::CreateStatic( &FLocal::Execute ) );
	}
}

void UUnrealProtocol_AssetOpen::CopyLink()
{
	TArray<FString> ClipboardStrings;
	for ( const FAssetData& AssetData : UEditorUtilityLibrary::GetSelectedAssetData() )
	{
		ClipboardStrings.Emplace( FAssetOpen::BuildLink( AssetData ) );
	}

	FPlatformApplicationMisc::ClipboardCopy( *FString::Join( ClipboardStrings, TEXT( "\n" ) ) );
}
