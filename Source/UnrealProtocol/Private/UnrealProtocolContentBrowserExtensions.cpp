// Fill out your copyright notice in the Description page of Project Settings.

#include "UnrealProtocolContentBrowserExtensions.h"

#include "ContentBrowserModule.h"
#include "UnrealProtocol_AssetOpen.h"

#define LOCTEXT_NAMESPACE "UnrealProtocolEd"

class FUnrealProtocolContentBrowserCommands final : public TCommands<FUnrealProtocolContentBrowserCommands>
{
public:
	FUnrealProtocolContentBrowserCommands() :
		TCommands( TEXT( "UnrealProtocolEditorCommands" ),
			NSLOCTEXT( "Contexts", "FUnrealProtocolEditorModule", "Unreal Protocol Editor Plugin" ), FName(),
			FAppStyle::GetAppStyleSetName() )
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override
	{
		UI_COMMAND( CopyLink, "Copy Link", "copy custom protocol link.", EUserInterfaceActionType::Button,
			FInputChord( EModifierKey::Control | EModifierKey::Shift, EKeys::C ) );
	}

public:
	TSharedPtr<FUICommandInfo> CopyLink;
};

namespace UnrealProtocol::ContentBrowserExtensions
{
static FDelegateHandle DH_ContentBrowserCommandExtender;

void OnExtendContentBrowserCommands( TSharedRef<FUICommandList> CommandList, FOnContentBrowserGetSelection GetSelectionDelegate )
{
	CommandList->MapAction( FUnrealProtocolContentBrowserCommands::Get().CopyLink, FExecuteAction::CreateLambda( [] {	 //
		UUnrealProtocol_AssetOpen::StaticClass()->GetDefaultObject<UUnrealProtocol_AssetOpen>()->CopyLink();
	} ) );
}

#undef LOCTEXT_NAMESPACE

void FHooks::Install()
{
	FUnrealProtocolContentBrowserCommands::Register();

	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>( TEXT( "ContentBrowser" ) );

	DH_ContentBrowserCommandExtender =
		ContentBrowserModule.GetAllContentBrowserCommandExtenders()
			.Emplace_GetRef( FContentBrowserCommandExtender::CreateStatic( &OnExtendContentBrowserCommands ) )
			.GetHandle();
}

void FHooks::Uninstall()
{
	FContentBrowserModule& ContentBrowserModule =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>( TEXT( "ContentBrowser" ) );

	ContentBrowserModule.GetAllContentBrowserCommandExtenders().RemoveAll(
		[]( const FContentBrowserCommandExtender& Delegate ) {	  //
		return Delegate.GetHandle() == DH_ContentBrowserCommandExtender;
	} );

	FUnrealProtocolContentBrowserCommands::Unregister();
}
}	 // namespace UnrealProtocol::ContentBrowserExtensions
