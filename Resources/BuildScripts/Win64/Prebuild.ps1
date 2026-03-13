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
$EngineVersionData = ((Get-Content "$Env:EngineDir/Build/Build.version") | ConvertFrom-JSON)
$MajorVersion = $EngineVersionData.MajorVersion
$MinorVersion = $EngineVersionData.MinorVersion
$PatchVersion = $EngineVersionData.PatchVersion
# Find the Python executable that ships with Unreal Editor.
if ($MajorVersion -gt 4 -or $MinorVersion -ge 22)
{
    $PythonVersionSuffix = ""
    if ($MajorVersion -gt 4 -or $MinorVersion -ge 26)
    {
        $PythonVersionSuffix = "3"
    }
    $PythonExe = "$Env:EngineDir\Binaries\ThirdParty\Python$PythonVersionSuffix\$Env:HostPlatform\python.exe"
}
elseif ($MajorVersion -eq 4 -and $MinorVersion -ge 19)
{
    $PythonExe = "$Env:EngineDir\Source\ThirdParty\Python\$Env:HostPlatform\python.exe"
}
elseif ($MajorVersion -eq 4 -and $MinorVersion -ge 13)
{
    $PythonExe = "$Env:EngineDir\Extras\ThirdPartyNotUE\emsdk\$Env:HostPlatform\python\2.7.5.3_64bit\python.exe"
}
elseif ($MajorVersion -eq 4 -and $MinorVersion -ge 9)
{
    $PythonExe = "$Env:EngineDir\Source\ThirdParty\HTML5\emsdk\$Env:HostPlatform\python\2.7.5.3_64bit\python.exe"
}
# Fallback: Find the local Python installation, with a preference for python.exe
if (-Not (Test-Path "$PythonExe" -PathType Leaf))
{
    $PythonExe = (get-command -ErrorAction SilentlyContinue python.exe).Path
    if (-Not (Test-Path "$PythonExe" -PathType Leaf))
    {
        $PythonExe = (get-command -ErrorAction SilentlyContinue python3.exe).Path
        if (-Not (Test-Path "$PythonExe" -PathType Leaf))
        {
            Write-Error "Failed to find Python! Please be sure it's installed to execute prebuild scripts."
        }
        else
        {
            # On modern Windows systems, python3.exe is a shim that notifies you whether it has been installed.
            # Running the python3 shim without arguments will open a page in Windows Store for installation.
            # Test whether python3.exe is just a shim or if it's actually installed
            $PythonVersionOutput = & "$PythonExe" --version 2>&1 | Out-String
            if ("$PythonVersionOutput" -match ".*Python was not found")
            {
                Write-Warning "Python is not installed! Opening Windows Store page..."
                & "$PythonExe"
            }
        }
    }
}
$Env:UEMajorVersion = $MajorVersion
$Env:UEMinorVersion = $MinorVersion
$Env:UEPatchVersion = $PatchVersion
& "$PythonExe" "$Env:PluginDir\Resources\BuildScripts\Prebuild.py"