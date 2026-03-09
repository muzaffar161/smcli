[Setup]
AppName=smcli
AppVersion=1.0.0
DefaultDirName=C:\smcli
OutputDir=.
OutputBaseFilename=smcli-setup
DisableDirPage=no
ChangesEnvironment=yes

[Files]
; Устанавливаем в подпапку bin внутри C:\smcli
Source: "smcli.exe"; DestDir: "{app}\bin"; Flags: ignoreversion

[Icons]
Name: "{group}\smcli"; Filename: "{app}\bin\smcli.exe"

[Registry]
; Добавляем C:\smcli\bin в пользовательский PATH
Root: HKCU; Subkey: "Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}\bin"; Check: NeedsAddPath

[Code]
function NeedsAddPath(): Boolean;
var
  OldPath: String;
  NewPath: String;
begin
  if not RegQueryStringValue(HKEY_CURRENT_USER, 'Environment', 'Path', OldPath) then
  begin
    Result := True;
    exit;
  end;
  NewPath := ExpandConstant('{app}\bin');
  // Проверяем, нет ли уже такого пути в PATH
  Result := Pos(';' + UpperCase(NewPath), ';' + UpperCase(OldPath)) = 0;
end;
