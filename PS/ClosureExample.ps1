$i = 10
$block = { "Value of i is $i" } # .GetNewClosure()
$i = 1
&$block

# params example
$block = { param ([string]$message, [int]$something) Write-Host "Hello!"; Write-Host (5 + $something);  }
&$block -message "blah" -something 3

Function Do-Something {
	[CmdletBinding()]
	param(
		[Parameter(Mandatory=$True,
		ValueFromPipeline=$True)]
		[string[]]$computername
	)
	BEGIN {}
	PROCESS {
		Foreach ($computer in $computername) {
			# use $computer here
		}
}
	END {}
}