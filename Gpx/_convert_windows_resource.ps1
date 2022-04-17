
$ExecutionPath = $MyInvocation.MyCommand.Definition
$ExecutionFolderPath = Split-Path -Parent $MyInvocation.MyCommand.Definition
$ScriptFolderPath = $PSScriptRoot;

$Env:Path += ";"
$Env:Path += $ScriptFolderPath + "\..\_Release\"

Get-ChildItem $ScriptFolderPath\UnitTest\Vertices\ | ForEach-Object -Process {
    if ($_ -is [System.IO.FileInfo])
    {
        if ($_.name.contains(".vertices.json"))
        {
            $FileNameWithoutExtension = [io.path]::GetFileNameWithoutExtension($_.name);
            $FileNameWithoutExtension = [io.path]::GetFileNameWithoutExtension($FileNameWithoutExtension);
            $args = "convert {0}\UnitTest\Vertices\{1} {0}\UnitTest\Vertices\{2}.p9vb" -f $ScriptFolderPath, $_, $FileNameWithoutExtension
            $proc = Start-Process Gpx_GpxTool.exe -ArgumentList $args -NoNewWindow -Wait -PassThru
            if ($proc.ExitCode -eq 0)
            {
                Write-Host("[",$_.name,"] convert done.");
            }
        }
    }
}

Get-ChildItem $ScriptFolderPath\UnitTest\Texture\ | ForEach-Object -Process {
    if ($_ -is [System.IO.FileInfo])
    {
        if (!$_.name.contains(".p9tt"))
        {
            $FileNameWithoutExtension = [io.path]::GetFileNameWithoutExtension($_.name);
            $args = "convert {0}\UnitTest\Texture\{1} {0}\UnitTest\Texture\{2}.p9tt" -f $ScriptFolderPath, $_, $FileNameWithoutExtension
            $proc = Start-Process Gpx_GpxTool.exe -ArgumentList $args -NoNewWindow -Wait -PassThru
            if ($proc.ExitCode -eq 0)
            {
                Write-Host("[",$_.name,"] convert done.");
            }
        }
    }
}
