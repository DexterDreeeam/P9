
./setup_environment.ps1

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Ge.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Ge/Ge.sln  /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Ge/Ge.sln  /p:configuration="release" /p:Platform="x64" /t:Rebuild
