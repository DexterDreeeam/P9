
./setup_environment.ps1

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Sdk.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Sdk/Sdk.sln  /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Sdk/Sdk.sln  /p:configuration="release" /p:Platform="x64" /t:Rebuild
