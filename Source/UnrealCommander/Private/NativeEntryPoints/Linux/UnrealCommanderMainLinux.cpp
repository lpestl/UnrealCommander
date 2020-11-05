// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealCommanderApp.h"
#include "UnixCommonStartup.h"

int main(int argc, char *argv[])
{
	return CommonUnixMain(argc, argv, &RunUnrealCommander);
}
