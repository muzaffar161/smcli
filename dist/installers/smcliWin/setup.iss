[Setup]
AppName=smcli
AppVersion=1.0.0
DefaultDirName={pf}\smartTerm
OutputDir=.
OutputBaseFilename=smcli-setup
DisableDirPage=no

[Files]
; путь к exe после сборки (bin\smcli.exe в корне репозитория)
Source: "..\..\..\bin\smcli.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\smartTerm CLI"; Filename: "{app}\smcli.exe"
Name: "{userdesktop}\smartTerm CLI"; Filename: "{app}\smcli.exe"