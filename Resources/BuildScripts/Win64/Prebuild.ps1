$EngineVersionData = ((Get-Content "$Env:EngineDir/Build/Build.version") | ConvertFrom-JSON)
$MajorVersion = $EngineVersionData.MajorVersion
$MinorVersion = $EngineVersionData.MinorVersion
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
& "$PythonExe" "$Env:PluginDir\Resources\BuildScripts\Prebuild.py"