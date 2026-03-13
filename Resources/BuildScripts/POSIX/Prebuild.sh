#!/usr/bin/env bash

# Version Macros for supporting projects that span multiple versions of Unreal.
# https://github.com/sbseltzer/VersionMacros
# 
# Copyright Sam Seltzer-Johnston 2026. All Rights Reserved.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
# Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Deduce engine version from build environment
BuildVersionFile="$EngineDir/Build/Build.version"
MajorVersion=$(< "$BuildVersionFile" grep -o '"MajorVersion".*,' | grep -o '\d*')
MinorVersion=$(< "$BuildVersionFile" grep -o '"MinorVersion".*,' | grep -o '\d*')
PatchVersion=$(< "$BuildVersionFile" grep -o '"PatchVersion".*,' | grep -o '\d*')
# Find the local Python installation, with a preference for Python3
PythonExe=$(command -v python3)
if [ ! -f "$PythonExe" ]; then
    PythonExe=$(command -v python)
    if [ ! -f "$PythonExe" ]; then
        echo "Failed to find Python! Please be sure it's installed to execute prebuild scripts."
        exit 1
    fi
fi
export UEMajorVersion=$MajorVersion
export UEMinorVersion=$MinorVersion
export UEPatchVersion=$PatchVersion
"$PythonExe" "$PluginDir/Resources/BuildScripts/Prebuild.py"