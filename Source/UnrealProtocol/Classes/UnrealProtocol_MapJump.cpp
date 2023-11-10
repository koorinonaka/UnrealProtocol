// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealProtocol_MapJump.h"

#include "HAL/PlatformApplicationMisc.h"
#include "Subsystems/UnrealEditorSubsystem.h"

namespace UnrealProtocol
{
struct FMapJumpCallback
{
	FMapJumpCallback()
	{
		static FMapJump Singleton;
		UUnrealProtocol::SetCallback(
			FMapJump::Path.GetData(), UUnrealProtocol::FURIDelegate::CreateRaw( &Singleton, &FMapJump::Execute ) );
	}
} GMapJumpCallback;

FString FMapJump::BuildLink( const FUnrealProtocol_MapJumpContext& Context )
{
	FString ContextString;
	Serialize( Context, ContextString );

	return UnrealProtocol::BuildLink(
		FString::Printf( TEXT( "%s?ctx=%s" ), Path.GetData(), *FGenericPlatformHttp::UrlEncode( ContextString ) ) );
}

void FMapJump::Execute( const FString& RawURL )
{
	TOptional<FString> ContextString = FGenericPlatformHttp::GetUrlParameter( RawURL, TEXT( "ctx" ) );
	if ( !ContextString.IsSet() )
	{
		UUnrealProtocol::ShowNotification(
			FString::Printf( TEXT( "[%s]:\nno specified context." ), Path.GetData() ), ENotification::Fail );
		return;
	}

	FUnrealProtocol_MapJumpContext Context;
	if ( !Deserialize( FGenericPlatformHttp::UrlDecode( ContextString.GetValue() ), Context ) )
	{
		UUnrealProtocol::ShowNotification(
			FString::Printf( TEXT( "[%s]:\ninvalid context." ), Path.GetData() ), ENotification::Fail );
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
}	 // namespace UnrealProtocol

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
				FPlatformApplicationMisc::ClipboardCopy(
					*UnrealProtocol::FMapJump::BuildLink( { EditorWorld, CameraLocation, CameraRotation } ) );
			}
		}
	}
}