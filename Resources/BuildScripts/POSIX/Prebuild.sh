#!/usr/bin/env bash
BuildVersionFile="$EngineDir/Build/Build.version"
MajorVersion=$(< "$BuildVersionFile" grep -o '"MajorVersion".*,' | grep -o '\d*')
MinorVersion=$(< "$BuildVersionFile" grep -o '"MinorVersion".*,' | grep -o '\d*')
# Execute Prebuild.py using the Python binary that ships with Unreal Editor.
PythonVersion=""
if [ "$MajorVersion" -gt "4" ] || [ "$MinorVersion" -ge "26" ]; then
  PythonVersion="3"
fi
# Per-version Python executable paths
if [ "$MajorVersion" -gt "4" ] || [ "$MinorVersion" -ge "22" ]; then
  PythonExe="$EngineDir/Binaries/ThirdParty/Python$PythonVersion/$HostPlatform/bin/python$PythonVersion"
elif [ "$MajorVersion" -gt "4" ] || [ "$MinorVersion" -ge "19" ]; then
  PythonExe="$EngineDir/Source/ThirdParty/Python$PythonVersion/$HostPlatform/bin/python"
elif [ "$MajorVersion" -gt "4" ] || [ "$MinorVersion" -ge "13" ]; then
  PythonExe="$EngineDir/Extras/ThirdPartyNotUE/emsdk/$HostPlatform/python/2.7.5.3_64bit/bin/python"
elif [ "$MajorVersion" -gt "4" ] || [ "$MinorVersion" -ge "9" ]; then
  PythonExe="$EngineDir/Source/ThirdParty/HTML5/emsdk/$HostPlatform/python/2.7.5.3_64bit/bin/python"
fi
# Fall back to Python executable in environment PATH.
if [ ! -f "$PythonExe" ]; then
    PythonExe="python"
fi
"$PythonExe" "$PluginDir/Resources/BuildScripts/Prebuild.py"