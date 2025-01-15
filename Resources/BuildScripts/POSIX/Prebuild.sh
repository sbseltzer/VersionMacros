#!/usr/bin/env bash
BuildVersionFile="$EngineDir/Build/Build.version"
MajorVersion=$(< "$BuildVersionFile" grep -o '"MajorVersion".*,' | grep -o '\d*')
MinorVersion=$(< "$BuildVersionFile" grep -o '"MinorVersion".*,' | grep -o '\d*')
# Find the local Python installation, with a preference for Python3
PythonExe=$(where python3)
if [ ! -f "$PythonExe" ]; then
    PythonExe=$(where python)
    if [ ! -f "$PythonExe" ]; then
        echo "Failed to find Python! Please be sure it's installed to execute prebuild scripts."
        exit 1
    fi
fi
export UEMajorVersion=$MajorVersion
export UEMinorVersion=$MinorVersion
"$PythonExe" "$PluginDir/Resources/BuildScripts/Prebuild.py"