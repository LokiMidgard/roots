function create_directory
{
    param ($folder);
    if (-Not (Test-Path "$folder")) {
        New-Item -Path ".\" -Name "$folder" -ItemType "directory"
    }
}
