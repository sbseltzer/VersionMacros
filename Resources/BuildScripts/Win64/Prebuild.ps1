$EngineVersionData = ((Get-Content "$Env:EngineDir/Build/Build.version") | ConvertFrom-JSON)
$MajorVersion = $EngineVersionData.MajorVersion
$MinorVersion = $EngineVersionData.MinorVersion
# Execute Prebuild.py using the Python binary that ships with Unreal Editor.
$PythonVersion = ""
if ($MajorVersion -gt 4 -or $MinorVersion -ge 26)
{
    $PythonVersion = "3"
}
$PythonExe = "$Env:EngineDir\Binaries\ThirdParty\Python$PythonVersion\Win64\python.exe"
& "$PythonExe" "$Env:PluginDir\Resources\BuildScripts\Prebuild.py"