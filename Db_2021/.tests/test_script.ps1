
for ($i=1; $i -le 1000000000; $i++)
{
    Get-ChildItem ./ -Filter *.json | ForEach-Object -Process{
        Write-Host($_.name);
        &"./Db_2021_EntryPoint.exe" 192.168.0.106 $_.name
    }
}
