// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace UnrealProtocol::ContentBrowserExtensions
{
class FHooks
{
public:
	static void Install();
	static void Uninstall();
};
}	 // namespace UnrealProtocol::ContentBrowserExtensions
