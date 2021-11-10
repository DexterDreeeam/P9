$Env:Path += ";C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\amd64"

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Sdk.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Sdk/Sdk.sln             /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Sdk/Sdk.sln             /p:configuration="release" /p:Platform="x64" /t:Rebuild

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Db_2021.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Db_2021/Db_2021.sln     /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Db_2021/Db_2021.sln     /p:configuration="release" /p:Platform="x64" /t:Rebuild

Copy-Item -Path "./.exe.release/*.exe" -Destination "./_Release/" -Recurse
