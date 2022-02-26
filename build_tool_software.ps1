
./setup_environment.ps1

Copy-Item -Path "./_BuildSettings_WindowsMsvc/ToolSoftware.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./ToolSoftware/ToolSoftware.sln  /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./ToolSoftware/ToolSoftware.sln  /p:configuration="release" /p:Platform="x64" /t:Rebuild
