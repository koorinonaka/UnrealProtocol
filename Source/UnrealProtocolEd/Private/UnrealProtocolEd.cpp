// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/ModuleManager.h"
#include "UnrealProtocolContentBrowserExtensions.h"

class FUnrealProtocolEdModule final : public IModuleInterface
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

IMPLEMENT_GAME_MODULE( FUnrealProtocolEdModule, UnrealProtocolEd )
