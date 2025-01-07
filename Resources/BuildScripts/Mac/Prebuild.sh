#!/usr/bin/env bash
BuildVersionFile="$EngineDir/Build/Build.version"
MajorVersion=$(< "$BuildVersionFile" grep -o '"MajorVersion".*,' | grep -o '\d*')
MinorVersion=$(< "$BuildVersionFile" grep -o '"MinorVersion".*,' | grep -o '\d*')
# Execute Prebuild.py using the Python binary that ships with Unreal Editor.
PythonVersion=""
if [ "$MajorVersion" -gt "4" ] || [ "$MinorVersion" -ge "26" ]; then
  PythonVersion="3"
fi
PythonExe="$EngineDir/Binaries/ThirdParty/Python$PythonVersion/Mac/bin/python$PythonVersion"
# Fall back to Python executable in environment PATH.
if [ ! -f "$PythonExe" ]; then
    PythonExe="python"
fi
$PythonExe "$PluginDir/Resources/BuildScripts/Prebuild.py"