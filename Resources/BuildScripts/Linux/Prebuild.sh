#!/usr/bin/env bash
BuildVersionFile="$EngineDir/Build/Build.version"
MajorVersion=$(< "$BuildVersionFile" grep -o '"MajorVersion".*,' | grep -o '\d*')
MinorVersion=$(< "$BuildVersionFile" grep -o '"MinorVersion".*,' | grep -o '\d*')
# Execute Prebuild.py using the Python binary that ships with Unreal Editor.
PythonVersion=""
if [ "$MajorVersion" -gt "4" ] || [ "$MinorVersion" -ge "26" ]; then
  PythonVersion="3"
fi
PythonExe="$EngineDir/Binaries/ThirdParty/Python$PythonVersion/Linux/bin/python$PythonVersion"
$PythonExe "$PluginDir/Resources/BuildScripts/Prebuild.py"