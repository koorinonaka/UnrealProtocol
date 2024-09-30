// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealProtocol_MapJump.h"

#include "HAL/PlatformApplicationMisc.h"
#include "Subsystems/UnrealEditorSubsystem.h"

namespace UnrealProtocol
{
FString FMapJump::BuildLink( const FUnrealProtocol_MapJumpContext& Context )
{
	FString ContextString;
	Serialize( Context, ContextString );

	return UnrealProtocol::BuildLink(
		FString::Printf( TEXT( "%s?ctx=%s" ), Path.GetData(), *FGenericPlatformHttp::UrlEncode( ContextString ) ) );
}
}	 // namespace UnrealProtocol

using namespace UnrealProtocol;

UUnrealProtocol_MapJump::UUnrealProtocol_MapJump()
{
	struct FLocal
	{
		static void Execute( const FString& RawURL )
		{
			TOptional<FString> ContextString = FGenericPlatformHttp::GetUrlParameter( RawURL, TEXT( "ctx" ) );
			if ( !ContextString.IsSet() )
			{
				UUnrealProtocol::ShowNotification(
					FString::Printf( TEXT( "[%s]:\nno specified context." ), FMapJump::Path.GetData() ), ENotification::Fail );
				return;
			}

			FUnrealProtocol_MapJumpContext Context;
			if ( !Deserialize( FGenericPlatformHttp::UrlDecode( ContextString.GetValue() ), Context ) )
			{
				UUnrealProtocol::ShowNotification(
					FString::Printf( TEXT( "[%s]:\ninvalid context." ), FMapJump::Path.GetData() ), ENotification::Fail );
				return;
			}

			if ( GEditor )
			{
				auto* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
				check( AssetEditorSubsystem );

				AssetEditorSubsystem->OpenEditorForAsset( Context.World.ToSoftObjectPath() );

				auto* UnrealEditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
				check( UnrealEditorSubsystem );

				UnrealEditorSubsystem->SetLevelViewportCameraInfo( Context.CameraLocation, Context.CameraRotation );
			}
		}
	};

	if ( HasAnyFlags( RF_ClassDefaultObject ) )
	{
		GetMutableDefault<UUnrealProtocol>()->Delegates.FindOrAdd(
			FMapJump::Path.GetData(), UUnrealProtocol::FURIDelegate::CreateStatic( &FLocal::Execute ) );
	}
}

void UUnrealProtocol_MapJump::CopyLink()
{
	if ( GEditor )
	{
		auto* UnrealEditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
		check( UnrealEditorSubsystem );

		if ( UWorld* EditorWorld = UnrealEditorSubsystem->GetEditorWorld() )
		{
			FVector CameraLocation;
			FRotator CameraRotation;
			if ( UnrealEditorSubsystem->GetLevelViewportCameraInfo( CameraLocation, CameraRotation ) )
			{
				FPlatformApplicationMisc::ClipboardCopy( *FMapJump::BuildLink( { EditorWorld, CameraLocation, CameraRotation } ) );
			}
		}
	}
}
