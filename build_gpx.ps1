
./setup_environment.ps1

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Gpx.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Gpx/Gpx.sln  /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Gpx/Gpx.sln  /p:configuration="release" /p:Platform="x64" /t:Rebuild
