
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
