Get-ChildItem -Recurse -File -Filter "empty.txt" | ForEach-Object { Remove-Item $_.FullName }

Remove-Item $PSCommandPath