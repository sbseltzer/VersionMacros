$EngineVersionData = ((Get-Content "$Env:EngineDir/Build/Build.version") | ConvertFrom-JSON)
$MajorVersion = $EngineVersionData.MajorVersion
$MinorVersion = $EngineVersionData.MinorVersion
# Execute Prebuild.py using the Python binary that ships with Unreal Editor.
$PythonVersion = ""
if ($MajorVersion -gt 4 -or $MinorVersion -ge 26)
{
    $PythonVersion = "3"
}
# Per-version Python executable paths
if ($MajorVersion -gt 4 -or $MinorVersion -ge 22)
{
    $PythonExe = "$Env:EngineDir\Binaries\ThirdParty\Python$PythonVersion\$Env:HostPlatform\python.exe"
}
elseif ($MajorVersion -eq 4 -and $MinorVersion -ge 19)
{
    $PythonExe = "$Env:EngineDir\Source\ThirdParty\Python$PythonVersion\$Env:HostPlatform\python.exe"
}
elseif ($MajorVersion -eq 4 -and $MinorVersion -ge 13)
{
    $PythonExe = "$Env:EngineDir\Extras\ThirdPartyNotUE\emsdk\$Env:HostPlatform\python\2.7.5.3_64bit\python.exe"
}
elseif ($MajorVersion -eq 4 -and $MinorVersion -ge 9)
{
    $PythonExe = "$Env:EngineDir\Source\ThirdParty\HTML5\emsdk\$Env:HostPlatform\python\2.7.5.3_64bit\python.exe"
}
# Fall back to Python executable in environment PATH.
if (-Not (Test-Path "$PythonExe" -PathType Leaf))
{
    $PythonExe = "python.exe"
}
$Env:UEMajorVersion = $MajorVersion
$Env:UEMinorVersion = $MinorVersion
& "$PythonExe" "$Env:PluginDir\Resources\BuildScripts\Prebuild.py"