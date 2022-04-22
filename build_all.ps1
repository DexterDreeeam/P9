
./build_sdk_core.ps1
./build_sdk.ps1
./build_tool_software.ps1
./build_gpx.ps1
./build_ge.ps1

Copy-Item -Path "./.exe.release/*.exe" -Destination "./_Release/" -Recurse
