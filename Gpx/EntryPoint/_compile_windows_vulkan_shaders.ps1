
$ExecutionPath = $MyInvocation.MyCommand.Definition
$ExecutionFolderPath = Split-Path -Parent $MyInvocation.MyCommand.Definition
$ScriptFolderPath = $PSScriptRoot;

$Env:Path += ";"
$Env:Path += $ScriptFolderPath + "..\External\Vulkan\Windows\"

Get-ChildItem $ScriptFolderPath\Shader\Vulkan\Glsl\ | ForEach-Object -Process {
    if ($_ -is [System.IO.FileInfo])
    {
        # $FileNameWithoutExtension = [io.path]::GetFileNameWithoutExtension($_.name);
        glslc $ScriptFolderPath\Shader\Vulkan\Glsl\$_ -o $ScriptFolderPath\Shader\Vulkan\$_.spv
        Write-Host("[",$_.name,"] compile done.");
    }
}
