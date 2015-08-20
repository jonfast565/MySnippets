# Deploy Scripts Files
# Jon Fast
# Last Modified: 8/7/2015


## ENVIRONMENT VARS ##

$env:ReadySetCopyPath = "D:\BuildScripts\ReadySetCopy.Console.exe"

## UTILITY FUNCTIONS ##

# Deploy function (uses custom util)
function Deploy (
	[ValidateNotNullOrEmpty()]
	[string] $ConfigLocation,
	[ValidateNotNullOrEmpty()]
	[string] $ProfileName,
    [ValidateNotNullOrEmpty()]
    [string] $ProfileLevel,
    [ValidateNotNullOrEmpty()]
    [string] $FromLocation,
    [ValidateNotNullOrEmpty()]
    [string] $ToLocation,
    [string] $BuildNumber
) {
    $ExitCode = 0
    Try {
        Write-Host "Deploying files from $($FromLocation) to $($ToLocation)..."
        # this is for appending information about the to be pushed folder
        $ToLocationUrl = $ToLocation
        if (-not [string]::IsNullOrEmpty($BuildNumber)) {
            $ToLocationUrl = "$($ToLocation)/$($BuildNumber)/$($ProfileName)/"
        } else {
            $ToLocationUrl = "$($ToLocation)/$($ProfileName)/"
        }
        $CopyCommand = "$($env:ReadySetCopyPath) $($ConfigLocation) $($ProfileName) $($ProfileLevel) $($ToLocationUrl) $($FromLocation)"
        Invoke-Expression -Command "$($CopyCommand)"
        Write-Host "Deployment from $($FromLocation) to $($ToLocation) completed."
    }
    Catch [system.exception] {
        $Message = $_.Exception.Message
        Write-Host "An error occurred. $($Message)"
        $ExitCode = -1
    }
    exit $ExitCode
}

Deploy "\\devweb5\d$\BuildScripts\Config\Config.xml" "WebNCDR" "Release" "\\stgncdrweb1\d$\Websites\www.ncdr.com\WebNCDR\" "\\ncdrweb3\d$\ToBePushed" "PW08212015"