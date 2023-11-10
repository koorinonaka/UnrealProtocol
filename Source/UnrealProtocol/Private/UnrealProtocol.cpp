// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealProtocol.h"

#include "Editor.h"
#include "Framework/Notifications/NotificationManager.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Widgets/Notifications/SNotificationList.h"

IMPLEMENT_MODULE( FUnrealProtocolModule, UnrealProtocol )

namespace UnrealProtocol
{
static TMap<FName, UUnrealProtocol::FURIDelegate> Delegates;
}

void UUnrealProtocol::SetCallback( const FName Path, const FURIDelegate& Callback )
{
	UnrealProtocol::Delegates.Emplace( Path, Callback );
}

void UUnrealProtocol::Entry( const FString& URL )
{
	// 解析処理の都合上、dummyのhost名を付与
	const FString Path = FGenericPlatformHttp::GetUrlPath( "http://localhost/" + URL );

	const FURIDelegate* Delegate = UnrealProtocol::Delegates.Find( *Path );
	if ( !Delegate )
	{
		ShowNotification( FString::Printf( TEXT( "bad request: %s" ), *Path ), ENotification::Fail );
		return;
	}

	// ReSharper disable once CppExpressionWithoutSideEffects
	Delegate->ExecuteIfBound( URL );

	// エディタを強制的にアクティブ化
	if ( const TSharedPtr<SWindow> Window = FGlobalTabmanager::Get()->GetRootWindow() )
	{
		if ( Window->IsWindowMinimized() )
		{
			Window->Restore();
		}

		Window->HACK_ForceToFront();

		// 注意を引くためにWindow上部を点滅させる
		// Window->FlashWindow();
	}
}

bool UUnrealProtocol::ShowNotification( const FText Text, ENotification CompletionState, const float ExpireDuration )
{
	if ( CompletionState == ENotification::Fail )
	{
		UE_LOG( LogTemp, Warning, TEXT( "%s" ), *Text.ToString() );
	}

	FNotificationInfo NotificationInfo( Text );
	NotificationInfo.ExpireDuration = ExpireDuration;
	NotificationInfo.bFireAndForget = true;
	NotificationInfo.bUseLargeFont = true;

	if ( const TSharedPtr<SNotificationItem> NotificationItem =
			 FSlateNotificationManager::Get().AddNotification( NotificationInfo ) )
	{
		NotificationItem->SetCompletionState( static_cast<SNotificationItem::ECompletionState>( CompletionState ) );
		return true;
	}

	return false;
}

bool UUnrealProtocol::ShowNotification( const FString Message, ENotification CompletionState, const float ExpireDuration )
{
	return ShowNotification( FText::FromString( Message ), CompletionState, ExpireDuration );
}

FString UnrealProtocol::BuildLink( const FString& URL )
{
	return FString::Printf( TEXT( "unreal://%s" ), *URL.TrimChar( '/' ) ).ToLower();
}