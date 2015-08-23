# Build Scripts Files
# Jon Fast
# Last Modified: 5/14/2015


## ENVIRONMENT VARS ##

$env:MSBuildPath = "C:\Windows\Microsoft.NET\Framework\v4.0.30319\msbuild.exe"
$env:RobocopyPath = "C:\Windows\System32\Robocopy.exe"
$env:NugetPath = "D:\BuildScripts\nuget.exe"

## UTILITY FUNCTIONS ##

# Build function
function Build (
	[ValidateNotNullOrEmpty()]
    [string] $ProjectFileLocation,
	[ValidateNotNullOrEmpty()]
    [string] $VisualStudioVersion,
	[ValidateNotNullOrEmpty()]
    [string] $DotNetVersion,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionConfiguration,
	[ValidateNotNull()]
    [bool]   $DeployOnBuildYesNo,
	[ValidateNotNull()]
    [bool]   $PerformClean,
    # Publish profile is optional, for web projects only
    [string] $PublishProfile
) {
    # Begin the build process
    Write-Host "Build Started..."
    [string] $MSBuildCommand = 
@"
    $env:MSBuildPath "$ProjectFileLocation" /m /nr:false /v:diag /tv:$DotNetVersion /p:VisualStudioVersion=$VisualStudioVersion /p:Configuration=$SolutionConfiguration /p:DeployOnBuild=$DeployOnBuildYesNo /maxcpucount:4
"@
    # check for the publish profile, and if exists, add to the command string
    if (-not [string]::IsNullOrEmpty($PublishProfile)) {
         $MSBuildCommand = "$($MSBuildCommand) /p:PublishProfile=$PublishProfile /P:TransformConfigFiles=true"
    }

    # check if a clean is required
    if ($PerformClean -eq $TRUE) {
        Write-Host "`r`nInvoking Clean: $($MSBuildCommand) /t:Clean"
        $MSBuildCleanResult = Invoke-Expression -Command "$($MSBuildCommand) /t:Clean"
        Write-Host ([system.string]::Join("`r`n", $MSBuildCleanResult))
        Write-Host "`r`nClean finished. Return code: $($LASTEXITCODE)"

        if ($LASTEXITCODE -ne 0) {
            $OverallExitCode = -1
        }
    }

    # invoke the build
    Write-Host "`r`nInvoking Build: $($MSBuildCommand) /t:Build"
    $MSBuildBuildResult = Invoke-Expression -Command "$($MSBuildCommand) /t:Build"
    Write-Host ([system.string]::Join("`r`n", $MSBuildBuildResult))
    Write-Host "`r`nBuild finished. Return code: $($LASTEXITCODE)"
    
    if ($LASTEXITCODE -ne 0) {
        return -1
    }

    return 0
}

function Copy-Files (
	[ValidateNotNullOrEmpty()]
    [string] $SolutionPublishLocation,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionDeployLocation
) {
    # Begin the copy process
    Write-Host "`r`nCopy started..."
    [string] $CopyCommand = 
@"
    $env:RobocopyPath "$SolutionPublishLocation" "$SolutionDeployLocation" /XD "AppData" /E /XO /FFT
"@
    $CopyResult = Invoke-Expression -Command $CopyCommand
    Write-Host ([system.string]::Join("`r`n", $CopyResult))
    Write-Host "`r`nCopy Finished. Return code: $($LASTEXITCODE)"
   
    # Housekeeping warning from 
    if ($LASTEXITCODE -eq 4) {
        Write-Host "`r`nHousekeeping may be required..."
    }

    # 8 or 16 inidicates an error that could possibly be fatal (see the docs)
    if ($LASTEXITCODE -gt 4) {
        return -1
    }
    return 0
}

function Eval-Exit (
    [ValidateNotNullOrEmpty()]
    [int] $OverallExitCode
) {
    # Check if the overall exit code is not equal to 
    if ($OverallExitCode -eq 0) {
        Write-Host "`r`nCompleted successfully."
    } else {
        Write-Host "`r`nCompleted with errors."
    }

    # Exit with the overall exit code
    exit $OverallExitCode
}

function Restore-Nuget-Packages
(
    [ValidateNotNullOrEmpty()]
    [string] $SolutionFileLocation
) {
    Write-Host "`r`n Restoring NuGet packages..."
    [string] $NugetCommand = 
@"
    $env:NugetPath restore "$SolutionFileLocation"
"@

    $NugetResult = Invoke-Expression -Command $NugetCommand
    # Nuget cmd doesn't return output :P
	# Write-Host ([system.string]::Join("`r`n", $NugetResult))
    Write-Host "`r`nNuGet restore finished. Return code: $($LASTEXITCODE)"
    
    if ($LASTEXITCODE -ne 0) {
        return -1
    }

    return 0
}

## PROCESS (EXITING) FUNCTIONS ##

function Restore-And-Build (
	[ValidateNotNullOrEmpty()]
    [string] $SolutionFileLocation,
	[ValidateNotNullOrEmpty()]
    [string] $ProjectFileLocation,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionPublishLocation,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionDeployLocation,
	[ValidateNotNullOrEmpty()]
    [string] $VisualStudioVersion,
	[ValidateNotNullOrEmpty()]
    [string] $DotNetVersion,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionConfiguration,
	[ValidateNotNull()]
    [bool]   $DeployOnBuildYesNo,
	[ValidateNotNull()]
    [bool]   $PerformClean,
    # Publish profile is optional, for web projects only
    [string] $PublishProfile
) {
    # Save an overall exit code, either 0 or 1
    $OverallExitCode = 0

    # Restore Nuget Packages
    $NugetExitCode = Restore-Nuget-Packages $SolutionFileLocation
    if ($NugetExitCode -ne 0) { $OverallExitCode = -1 } # can fail without issues

    # Build
    $BuildExitCode = Build $ProjectFileLocation $VisualStudioVersion $DotNetVersion $SolutionConfiguration $DeployOnBuildYesNo $PerformClean $PublishProfile
    if ($BuildExitCode -ne 0) { $OverallExitCode = -1 }
    
    # Exit
    Eval-Exit $OverallExitCode
}


# The basic build function
function Build-And-Copy (
	[ValidateNotNullOrEmpty()]
    [string] $SolutionFileLocation,
	[ValidateNotNullOrEmpty()]
    [string] $ProjectFileLocation,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionPublishLocation,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionDeployLocation,
	[ValidateNotNullOrEmpty()]
    [string] $VisualStudioVersion,
	[ValidateNotNullOrEmpty()]
    [string] $DotNetVersion,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionConfiguration,
	[ValidateNotNull()]
    [bool]   $DeployOnBuildYesNo,
	[ValidateNotNull()]
    [bool]   $PerformClean,
    # Publish profile is optional, for web projects only
    [string] $PublishProfile
) {
    # Save an overall exit code, either 0 or 1
    $OverallExitCode = 0

    # Restore Nuget Packages
    $NugetExitCode = Restore-Nuget-Packages $SolutionFileLocation
    if ($NugetExitCode -ne 0) { $OverallExitCode = -1 }

    # Build
    $BuildExitCode = Build $ProjectFileLocation $VisualStudioVersion $DotNetVersion $SolutionConfiguration $DeployOnBuildYesNo $PerformClean $PublishProfile
    if ($BuildExitCode -ne 0) { 
        $OverallExitCode = -1 
        Eval-Exit $OverallExitCode
    }
    
    # Copy
    $CopyExitCode = Copy-Files $SolutionPublishLocation $SolutionDeployLocation
    if ($CopyExitCode -ne 0) { $OverallExitCode = -1 }
    
    Eval-Exit $OverallExitCode
}

function Build-Only 
(
	[ValidateNotNullOrEmpty()]
    [string] $ProjectFileLocation,
	[ValidateNotNullOrEmpty()]
    [string] $VisualStudioVersion,
	[ValidateNotNullOrEmpty()]
    [string] $DotNetVersion,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionConfiguration,
	[ValidateNotNull()]
    [bool]   $DeployOnBuildYesNo,
	[ValidateNotNull()]
    [bool]   $PerformClean,
    # Publish profile is optional, for web projects only
    [string] $PublishProfile
) {
    $BuildExitCode = Build $ProjectFileLocation $VisualStudioVersion $DotNetVersion $SolutionConfiguration $DeployOnBuildYesNo $PerformClean $PublishProfile
    exit $BuildExitCode
}

function Copy-Only
(
	[ValidateNotNullOrEmpty()]
    [string] $SolutionPublishLocation,
	[ValidateNotNullOrEmpty()]
    [string] $SolutionDeployLocation
) {
    $CopyExitCode = Copy-Files $SolutionPublishLocation $SolutionDeployLocation
    exit $CopyExitCode
}