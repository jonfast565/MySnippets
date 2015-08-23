function Join-Url {
    param ([string[]] $Parts, [string] $Seperator = '')
    $search = '(?<!:)' + [regex]::Escape($Seperator) + '+'  # Replace multiples except in front of a colon for URLs.
    $replace = $Seperator
    $url = ($Parts | ? {$_ -and $_.Trim().Length}) -join $Seperator -replace $search, $replace
    if ($url.StartsWith("\") -and $Seperator -eq '\') { $url = "\$($url)" }
    return $url
}

function Backup (
    [ValidateNotNullOrEmpty()]
    [string] $FromLocation,
    [ValidateNotNullOrEmpty()]
    [string] $ToLocation,
    [ValidateNotNullOrEmpty()]
    [string] $BuildNumber,
    [string[]] $ItemsToBackup
) {
    $ExitCode = 0
    Try {
        $ItemsInFolder = Get-ChildItem $FromLocation | Where {$_.Attributes -eq 'Directory'} | Select -Expand Name
        ForEach ($item in $ItemsToBackup) {
            If ($ItemsInFolder -contains $item) {
                $FromLocationUrl = Join-Url @($FromLocation, $item) "\"
                $ToLocationUrl = Join-Url @($ToLocation, $BuildNumber, $item) "\"
                Write-Host "Backing up: $($item) from $($FromLocationUrl) to $($ToLocationUrl)..."
                $CopyResult = Copy-Item (Join-Url $FromLocationUrl, '*') $ToLocationUrl -Recurse -Force -PassThru
                Write-Host "Backup of $($item) completed."
				Write-Host "Output: \r\n $($CopyResult)."
            }
        }
    }
    Catch [system.exception] {
        $Message = $_.Exception.Message
        Write-Host "An error occurred. $($Message)"
        $ExitCode = -1
    }
    exit $ExitCode
}

function Backup-Specific-File (
    [ValidateNotNullOrEmpty()]
    [string] $FileName,
    [ValidateNotNullOrEmpty()]
    [string[]] $LocationNames,
    [ValidateNotNullOrEmpty()]
    [string] $FromLocation,
    [ValidateNotNullOrEmpty()]
    [string] $ToLocation,
    [ValidateNotNullOrEmpty()]
    [string] $BuildNumber,
    [string[]] $ItemsToBackup
) {
    $ExitCode = 0
    Try {
        ForEach ($LocationName in $LocationNames) {
            $FromLocationDir = Join-Url @("\\", $LocationName, $FromLocation)
            $ItemsInFolder = Get-ChildItem $FromLocationDir | Where {$_.Attributes -eq 'Directory'} | Select -Expand Name
            ForEach ($item in $ItemsToBackup) {
                If ($ItemsInFolder -contains $item) {
                    $FromLocationUrl = Join-Url @("\\", $LocationName, $FromLocation, $item)
                    $ToLocationUrl = Join-Url @($ToLocation, $BuildNumber, $item) "\"
                    Write-Host "Backing up: $($FileName) from $($FromLocationUrl) to $($ToLocationUrl)..."
                    $FromLocationFilename = Join-Url $FromLocationUrl, "\", $FileName
                    $UniqueFileName = "$($LocationName)_$($FileName)"
                    $ToLocationFilename = Join-Url $ToLocationUrl, "\", $UniqueFileName
                    $NewDirResult = New-Item -Type Directory -Force $ToLocationUrl
                    $CopyResult = Copy-Item $FromLocationFileName $ToLocationFilename -Recurse -Force -PassThru
                    Write-Host "Backup of $($item) completed."
    				Write-Host "Output: \r\n$($NewDirResult) \r\n $($CopyResult)."
                }
            }
        }
    }
    Catch [system.exception] {
        $Message = $_.Exception.Message
        Write-Host "An error occurred. $($Message)"
        $ExitCode = -1
    }
    exit $ExitCode
}