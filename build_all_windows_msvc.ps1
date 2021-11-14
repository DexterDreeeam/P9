$mspaths =@(
'C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\amd64'
'C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\amd64'
'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64'
)

for($i=0 ; $i -lt $mspaths.Length; $i++){
    If (Test-Path $mspaths[$i])
    {
         $Env:Path += ";"
         $Env:Path += $mspaths[$i]
         Write-Host $mspaths[$i] -Fore Green
         break
    }
}

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Sdk.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Sdk/Sdk.sln             /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Sdk/Sdk.sln             /p:configuration="release" /p:Platform="x64" /t:Rebuild

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Db_2021.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Db_2021/Db_2021.sln     /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Db_2021/Db_2021.sln     /p:configuration="release" /p:Platform="x64" /t:Rebuild

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Gpx.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Gpx/Gpx.sln             /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Gpx/Gpx.sln             /p:configuration="release" /p:Platform="x64" /t:Rebuild

Copy-Item -Path "./.exe.release/*.exe" -Destination "./_Release/" -Recurse
