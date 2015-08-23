# Pipeline example
function f {
        [CmdLetBinding()]
        Param (
            [Parameter(ValueFromPipeline=$true, Mandatory=$true, ValueFromPipelineByPropertyName=$true)]
            [string] $something,
            [Parameter(ValueFromPipeline=$true, Mandatory=$true, ValueFromPipelineByPropertyName=$true)]
            [string] $another,
            [Parameter(ValueFromPipeline=$true, Mandatory=$true, ValueFromPipelineByPropertyName=$true)]
            [string] $more
        )
    Begin {
        Write-Host "Started processing..."
    }
    Process {
        # Looping portion of the pipeline processor
        Write-Host "Iteration: "
        if (-not [string]::IsNullOrEmpty($something)) { Write-Host "Something: ", $something }
        if (-not [string]::IsNullOrEmpty($another)) { Write-Host "Another: ", $another }
        if (-not [string]::IsNullOrEmpty($more)) { Write-Host "More: ", $more }  
    }
    End {
        Write-Host "Finished processing..."
    }
}

# With PSObject (appears to bind object params to their respective values)
Write-Host (New-Object PSObject -Property @{ something = "something1"; another = "another2"; more = "more3" } | f)

# With array (appears to bind every param to exactly one value)
Write-Host ("something1", "another2", "more3" | f)
