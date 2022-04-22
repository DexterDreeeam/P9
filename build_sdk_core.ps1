
./setup_environment.ps1

Copy-Item -Path "./_BuildSettings_WindowsMsvc/SdkCore.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./SdkCore/SdkCore.sln  /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./SdkCore/SdkCore.sln  /p:configuration="release" /p:Platform="x64" /t:Rebuild
