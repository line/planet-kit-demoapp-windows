## Prepare basic variables
param(
    [string]$version="",
    [bool]$build=$false,
    [bool]$deploy=$false,
    [string]$deployMsg=""
)

$_buildRoot = $PSScriptRoot + "\..\LINEPlanetCall\"
$_appPackagesPath = $_buildRoot + "AppPackages\LINEPlanetCall\"
$_appxManifest = "Package.appxmanifest"
$_appVersionHeader = "constants/LinePlanetCallVersion.h"

$_branchName = (git rev-parse --abbrev-ref HEAD)
$_commitHash = (git rev-parse --short HEAD)


function Set-Package-Version {
    param (
        [string]$fileName,
        [string]$version
    )
    
    $versionParts = $version.split(".")
    $versionMajor = $versionParts[0]
    $versionMinor = $versionParts[1]
    $versionPatch = $versionParts[2]
    $versionBuild = $versionParts[3]

    $newVersion = $versionMajor + '.' + $versionMinor + '.' + $versionPatch + '.' + $versionBuild
    $versionFileName = $_buildRoot + $fileName
    
    $content = (gc $versionFileName) -join "`r`n"

    $callback = {
        param($match)
        $match.Groups[1].Value + 'Version="' + $newVersion + '"'
    }

    $identityRegex = [regex]'(\<Identity[^\>]*)Version=\"([0-9])+\.([0-9]+)\.([0-9]+)\.([0-9]+)\.*\"'
    $newContent = $identityRegex.Replace($content, $callback)

    [io.file]::WriteAllText($versionFileName, $newContent)

    Write-Host ("Written package version " + $version)
}

function Get-Package-Version {
    param (
        [string]$appxmanifest
    )
    $versionFileName = $_buildRoot + $appxmanifest

    $content = (gc $versionFileName) -join "`r`n"
    
    $identityRegex = [regex]'(\<Identity[^\>]*)Version=\"([0-9])+\.([0-9]+)\.([0-9]+)\.([0-9]+)\.*\"'
    $match = $content -split $identityRegex
    
    return $match[2] + '.' + $match[3] + '.' + $match[4] + '.' + $match[5]
}

function Set-Header-Version {
    param (
        [string]$fileName,
        [string]$version
    )
    $versionParts = $version.split(".")
    $versionMajor = $versionParts[0]
    $versionMinor = $versionParts[1]
    $versionPatch = $versionParts[2]
    $versionBuild = $versionParts[3]

    $newVersion = $versionMajor + '.' + $versionMinor + '.' + $versionPatch + '.' + $versionBuild

    $versionFileName = $_buildRoot + $fileName
    $content = (gc $versionFileName) -join "`r`n"

    $callback = {
        param($match)
        'AppVersion = L"' + $newVersion + '"'
    }
    
    $kitVersionRegex = [regex]'AppVersion = L\"([0-9])+\.([0-9]+)\.([0-9]+)\.([0-9]+)\.*\"'
    $newContent = $kitVersionRegex.Replace($content, $callback)
    
    [io.file]::WriteAllText($versionFileName, $newContent)

    Write-Host ("Written app header version " + $version)
}

function Run-VS2022-MSBuild-Environment {
    ## https://stackoverflow.com/a/2124759
    ##
    pushd "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools"
    cmd /c "VsMSBuildCmd.bat&set" |
    foreach {
        if ($_ -match "=") {
            $v = $_.split("=", 2); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])" 
        }
    }
    popd
    Write-Host "`nVisual Studio 2022 Command Prompt variables set." -ForegroundColor Yellow
}

function Build-LINEPlanetCall {
    Run-VS2022-MSBuild-Environment
    
    $platform = "x64"
    
    $buildTarget = "`"LINEPlanetCall`""
    $appxPlatformPath = $_appPackagesPath + $_appxVersion + "\" + $platform + "\"

    pushd ($_win32Root + "/build/")
    $props = @{
        VisualStudioVersion='17.0'
        Configuration='Release'
        Platform=$platform
        AppxBundle='Always'
        AppxBundlePlatforms='"x86|x64"'
        AppxPackageSigningEnabled='true'
        AppxSymbolPackageEnabled='true'
        AppxPackageDir='"' + $appxPlatformPath + '"'
        PackageCertificateKeyFile='LINEPlanetKit_TemporaryKey.pfx'
        UapAppxPackageBuildMode='SideloadOnly'
        GenerateAppxPackageOnBuild='true'
    }

    $propsString = ($props.GetEnumerator() | ForEach-Object { "/p:" + $_.Name + "=" + $_.Value }) -join " "
    Write-Host ("MSBuild options: " + $propsString)

    Invoke-Expression ("MSBuild LINEPlanetCall.sln /t:" + $buildTarget + " " + $propsString)
    popd

    Write-Host ("Built at: " + $_appPackagesPath)
}

function Copy-Symbol {
    $symbolPath = $_buildRoot + "LINEPlanetCall\x64\Release\Symbols\LINEPlanetCall.pdb"
    $targetPath = $_buildRoot + "AppPackages\LINEPlanetCall\" + $_appxVersion + "\x64\LINEPlanetCall_" + $_appxVersion + "_Test"
    
    Write-Host ("symbolPath at: " + $symbolPath)
    Write-Host ("targetPath at: " + $targetPath)
    
    Copy-Item $symbolPath -Destination $targetPath
}

function Zip-LINEPlanetCall {
    param (
        [string]$appxPath
    )

    $appxVersionPath = $appxPath + $_appxVersion

    $sourceFilePattern = ("LINEPlanetCall*.*")
    $sourceFiles = (Get-ChildItem -Path $appxVersionPath -Filter $sourceFilePattern -Recurse -Include *.msix,*.msixsym,*.cer,*.pdb)
    
    if ($sourceFiles.Count -eq 0) {
        Write-Host ($_appxVersion + " binary not exists!") -ForegroundColor Red
        return ""
    }

    Write-Host ("Found app files: `n")
    Write-Host ($sourceFiles -join "`n") -ForegroundColor Yellow

    $outputZip = $appxVersionPath + "/LINEPlanetCall_" + $_appxVersion + ".zip"

    Compress-Archive -Path $sourceFiles -DestinationPath $outputZip -Force

    Write-Host ("Archived zip at: " + $outputZip)
    return $outputZip
}

function Upload-LINEPlanetCall {
    $linePlanetCallZipPath = Zip-LINEPlanetCall $_appPackagesPath
    
    if ([string]::IsNullOrEmpty($linePlanetCallZipPath)) {
        return
    }
    
    $zipDate = (Get-Item -Path $linePlanetCallZipPath).LastWriteTime.ToString("yyyy.MM.dd")
    $zipFile = (Split-Path -Path $linePlanetCallZipPath -Leaf)

    $version = ($zipFile.SubString(0, $zipFile.LastIndexOf(".")) -split "_")[1]
    $versionParts = $version.split(".")
    $version_major_minor = $versionParts[0] + "." + $versionParts[1]

    $phase = "ALPHA"
    $branch = $_branchName.ToLower()

    $baseBranchName = ($branch -split "/")[0]
    if ($baseBranchName -eq "release") {
        $phase = "RELEASE"
        $branch = $_branchName.ToLower().Replace("/", "_")
    }
    else {
        $branch = "release_" + $version_major_minor
    }

    $releaseNote = $version + "-" + $zipDate + "-" + $_commitHash + " " + $_branchName + " " + $deployMsg
    $accessToken = "My00MDdiLTlmZTMtYmUzNzlkN2Q2OGQyNTc2OTkyYmYtOTY0NS00Nzk0LTg5Zjct"

    $curlOptions = @(
        '-X', 'POST',
        '-F', 'productCode=line-planet-call',
        '-F', 'deviceCode=Windows',
        '-F', ('phase=' + $phase),
        '-F', ('version=' + $version),
        '-F', ('branch=' + $branch),
        '-F', ('description=' + '"' + $releaseNote + '"')
        '-F', ('files=@' + '"' + $linePlanetCallZipPath + '"' ),
        '-H', ('JADACCESSTOKEN:' + $accessToken),
        'http://admin.appdeploy.navercorp.jp/api/insertBuild'
    )

    Write-Host ("appdeploy options: " + ($curlOptions -join " "))

    Invoke-Expression ("curl.exe " + $curlOptions)
}


## Start main script from here
pushd $_buildRoot

if ($version) {
    Set-Package-Version $_appxManifest $version
    Set-Header-Version $_appVersionHeader $version
}
$_appxVersion = (Get-Package-Version $_appxManifest)

if ($build -eq $true) {
    Build-LINEPlanetCall
    Copy-Symbol
}

if ($deploy -eq $true) {
	# upload LINE Planet Call
	Upload-LINEPlanetCall
}


popd
