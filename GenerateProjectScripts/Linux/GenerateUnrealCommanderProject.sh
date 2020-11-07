#!/bin/sh

set -e

bash /home/lpestl/Projects/UnrealEngine/GenerateProjectFiles.sh -makefile -project="/home/lpestl/Projects/UnrealCommander/UnrealCommander.uproject" -game -engine -progress
# bash /home/lpestl/Projects/UnrealEngine/GenerateProjectFiles.sh -VSCode -project="/home/lpestl/Projects/UnrealCommander/UnrealCommander.uproject" -game -engine -progress

# bash "/home/lpestl/Projects/UnrealEngine/Engine/Build/BatchFiles/Linux/Build.sh" UnrealCommander Linux Shipping  -project="/home/lpestl/Projects/UnrealCommander/UnrealCommander.uproject"