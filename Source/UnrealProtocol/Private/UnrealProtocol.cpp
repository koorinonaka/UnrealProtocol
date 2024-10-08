// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealProtocol.h"

#include "Framework/Notifications/NotificationManager.h"
#include "UnrealProtocolContentBrowserExtensions.h"
#include "Widgets/Notifications/SNotificationList.h"

class FUnrealProtocolModule final : public IModuleInterface
{
	virtual void StartupModule() override
	{
		if ( IsRunningCommandlet() )
		{
			return;
		}

		UnrealProtocol::ContentBrowserExtensions::FHooks::Install();
	}

	virtual void ShutdownModule() override	  //
	{
		UnrealProtocol::ContentBrowserExtensions::FHooks::Uninstall();
	}
};

IMPLEMENT_MODULE( FUnrealProtocolModule, UnrealProtocol )

void UUnrealProtocol::Entry( const FString& URL )
{
	// 解析処理の都合上、dummyのhost名を付与
	const FString Path = FGenericPlatformHttp::GetUrlPath( "http://localhost/" + URL );

	const FURIDelegate* Delegate = GetDefault<UUnrealProtocol>()->Delegates.Find( *Path );
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

bool UUnrealProtocol::ShowNotification( const FString& Message, ENotification CompletionState, const float ExpireDuration )
{
	return ShowNotification( FText::FromString( Message ), CompletionState, ExpireDuration );
}

FName UUnrealProtocolSettings::GetCategoryName() const
{
	return TEXT( "Plugins" );
}

FText UUnrealProtocolSettings::GetSectionText() const
{
	return NSLOCTEXT( "UnrealProtocol", "UnrealProtocolSettingsSection", "Unreal Protocol" );
}

FString UnrealProtocol::BuildLink( const FString& URL )
{
	return FString::Printf( TEXT( "%s://%s" ),	  //
		*GetDefault<UUnrealProtocolSettings>()->GetCustomScheme().ToString(), *URL.TrimChar( '/' ) ).ToLower();
}
