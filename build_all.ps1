
./build_sdk.ps1
./build_db_2021.ps1
./build_gpx.ps1
./build_ge.ps1

Copy-Item -Path "./.exe.release/*.exe" -Destination "./_Release/" -Recurse