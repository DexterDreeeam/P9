
$ExecutionPath = $MyInvocation.MyCommand.Definition
$ExecutionFolderPath = Split-Path -Parent $MyInvocation.MyCommand.Definition
$ScriptFolderPath = $PSScriptRoot;

$Env:Path += ";"
$Env:Path += $ScriptFolderPath + "\External\Vulkan\Windows\"

Get-ChildItem $ScriptFolderPath\UnitTest\Shader\Vulkan\Glsl\ | ForEach-Object -Process {
    if ($_ -is [System.IO.FileInfo])
    {
        # $FileNameWithoutExtension = [io.path]::GetFileNameWithoutExtension($_.name);
        glslc $ScriptFolderPath\UnitTest\Shader\Vulkan\Glsl\$_ -o $ScriptFolderPath\UnitTest\Shader\Vulkan\$_.spv
        Write-Host("[",$_.name,"] compile done.");
    }
}
