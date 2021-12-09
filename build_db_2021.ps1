
./setup_environment.ps1

Copy-Item -Path "./_BuildSettings_WindowsMsvc/Db_2021.inl" -Destination "./BuildSetting.inl" -force
msbuild  ./Db_2021/Db_2021.sln  /p:configuration="debug"   /p:Platform="x64" /t:Rebuild
msbuild  ./Db_2021/Db_2021.sln  /p:configuration="release" /p:Platform="x64" /t:Rebuild
