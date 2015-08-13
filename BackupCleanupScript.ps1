# Cleanup Scripts for old backups or other things
# Jon Fast : 6/2/2015

function Cleanup-Backups (
    [ValidateNotNullOrEmpty()]
    [string] $testPath
) {
    # start
    $ExitCode = 0
    Write-Host "Performing cleanup..."

    Try {
        # get all subdirectories
        $directories = Get-ChildItem -Path $testPath | Where-Object { $_.Attributes -eq "Directory" }

        # get all directories past the 3 month limit
        $3MonthDirectories = $directories | Where-Object {

            # only folders starting with PW or PH in the current directory
            if (-not [string]::IsNullOrEmpty($_.BaseName)) {
                return $_.BaseName.StartsWith("PW") -or $_.BaseName.StartsWith("PH")
            } else { return $null }
        } | Where-Object {

            # parse the folder names based on the last 8 chars
            # add another day to backup to improve the error margin with how DateTime recognizes the current day
            $backupLimitDate = (Get-Date).AddMonths(-3).AddDays(-1)
            $folderDate = [datetime]::ParseExact($_.BaseName.Substring(2, 8).Trim(), "MMddyyyy", [System.Globalization.CultureInfo]::InvariantCulture)
            return $folderDate -lt $backupLimitDate
        }

        # loop and delete
        if ($3MonthDirectories.Length -eq 0 -or $3MonthDirectories-eq $null) { Write-Host "Nothing to delete." }
        foreach($directory in $3MonthDirectories) {
            if ([string]::IsNullOrEmpty($directory) -ne $true) {
                Write-Host "Removing: $($directory.FullName)"
                Remove-Item $directory.FullName -Recurse -Force
            }
        }
    }
    Catch [system.exception]
    {
        Write-Host "An error occurred: $($_.Exception.Message)"
        $ExitCode = -1
    }

    # completed
    Write-Host "Done."
    exit $ExitCode
}