unit Storage;

interface

uses
	Forms, Controls, Registry, TypInfo, Classes;

// helpers
procedure LoadWindowState(Form: TForm);
procedure SaveWindowState(Form: TForm);

type
	TStorage = class
  public
  	constructor Create(Section: string);
    destructor Destroy; override;
    procedure WriteInteger(Ident: string; Value: Integer);
    procedure WriteBool(Ident: string; Value: Boolean);
    procedure WriteString(Ident: string; Value: string);
    function ReadInteger(Ident: string; Default: Longint): Integer;
    function ReadBool(Ident: string; Default: Boolean): Boolean;
    function ReadString(Ident: string; Default: string): string;
  protected
	  FStorage: TRegIniFile;
  	FSection: string;
  private
    procedure OpenKey;
    procedure CloseKey;
  end;

  TComponentStorage = class(TStorage)
  public
  	constructor Create(Section: string; Component: TComponent);
  	procedure SavePos;
    procedure LoadPos;
  	procedure SaveSize;
    procedure LoadSize;
   	procedure SaveProp(PropName: string);
    procedure LoadProp(PropName: string);
		procedure SaveComponentProp(Component: TComponent; PropName: string);
    procedure LoadComponentProp(Component: TComponent; PropName: string);
		procedure SaveCollectionItemsProp(Component: TComponent;
    	Collection: TCollection; PropName: string);
    procedure LoadCollectionItemsProp(Component: TComponent;
    	Collection: TCollection; PropName: string);
		procedure SaveAllComponentProp(ComponentClass: TComponentClass; PropName: string);
    procedure LoadAllComponentProp(ComponentClass: TComponentClass; PropName: string);

  protected
    FComponent: TComponent;
  end;

  TFormStorage = class(TComponentStorage)
  public
	  constructor Create(Form: TForm);
  	procedure SaveWindowState;
    procedure LoadWindowState;
  end;

implementation

uses
	SysUtils;

{ helpers }

procedure LoadWindowState(Form: TForm);
var
	FStorage: TFormStorage;
begin
	FStorage := TFormStorage.Create(Form);
  FStorage.LoadWindowState;
  FStorage.Free;
end;

procedure SaveWindowState(Form: TForm);
var
	FStorage: TFormStorage;
begin
	FStorage := TFormStorage.Create(Form);
  FStorage.SaveWindowState;
  FStorage.Free;
end;

{ TStorage }

constructor TStorage.Create(Section: string);
begin
  FSection := Section;
end;

destructor TStorage.Destroy;
begin
end;

procedure TStorage.OpenKey;
begin
	FStorage := TRegIniFile.Create('Software\Restoran');
end;

procedure TStorage.CloseKey;
begin
  FStorage.Free;
end;

function TStorage.ReadBool(Ident: string; Default: Boolean): Boolean;
begin
	OpenKey;
  Result := FStorage.ReadBool(FSection, Ident, Default);
  CloseKey;
end;

function TStorage.ReadInteger(Ident: string; Default: Integer): Integer;
begin
	OpenKey;
  Result := FStorage.ReadInteger(FSection, Ident, Default);
  CloseKey;
end;

procedure TStorage.WriteBool(Ident: string; Value: Boolean);
begin
	OpenKey;
	FStorage.WriteBool(FSection, Ident, Value);
  CloseKey;
end;

procedure TStorage.WriteInteger(Ident: string; Value: Integer);
begin
	OpenKey;
	FStorage.WriteInteger(FSection, Ident, Value);
  CloseKey;
end;

function TStorage.ReadString(Ident, Default: string): string;
begin
	OpenKey;
  Result := FStorage.ReadString(FSection, Ident, Default);
  CloseKey;
end;

procedure TStorage.WriteString(Ident, Value: string);
begin
	OpenKey;
	FStorage.WriteString(FSection, Ident, Value);
  CloseKey;
end;

{ TComponentStorage }

constructor TComponentStorage.Create(Section: string; Component: TComponent);
begin
	FComponent := Component;
  inherited Create(Section);
end;

procedure TComponentStorage.SaveProp(PropName: string);
var
	s: string;
begin
	try
		s := GetPropValue(FComponent, PropName);
	  WriteString(PropName, s);
  except
  end;
end;

procedure TComponentStorage.LoadProp(PropName: string);
var
	s: string;
begin
  try
		s := ReadString(PropName, '');
		SetPropValue(FComponent, PropName, s);
  except
  end;
end;

procedure TComponentStorage.LoadComponentProp(Component: TComponent;
  PropName: string);
var
	s: string;
begin
  try
		s := ReadString(Component.Name + '_' + PropName, '');
		SetPropValue(Component, PropName, s);
  except
  end;
end;

procedure TComponentStorage.SaveComponentProp(Component: TComponent;
  PropName: string);
var
	s: string;
begin
	try
		s := GetPropValue(Component, PropName);
	  WriteString(Component.Name + '_' + PropName, s);
  except
  end;
end;


procedure TComponentStorage.SaveAllComponentProp(ComponentClass: TComponentClass; PropName: string);
var
	i: Integer;
begin
	for i := 0 to FComponent.ComponentCount - 1 do
		if FComponent.Components[i] is ComponentClass then
			SaveComponentProp(FComponent.Components[i], PropName);
end;

procedure TComponentStorage.LoadAllComponentProp(ComponentClass: TComponentClass; PropName: string);
var
	i: Integer;
begin
	for i := 0 to FComponent.ComponentCount - 1 do
		if FComponent.Components[i] is ComponentClass then
			LoadComponentProp(FComponent.Components[i], PropName);
end;

procedure TComponentStorage.LoadSize;
begin
	LoadProp('Height');
	LoadProp('Width');
end;

procedure TComponentStorage.SaveSize;
begin
	SaveProp('Height');
	SaveProp('Width');
end;

procedure TComponentStorage.SavePos;
begin
	SaveProp('Top');
	SaveProp('Left');
  SaveSize;
end;

procedure TComponentStorage.LoadPos;
begin
	LoadProp('Top');
	LoadProp('Left');
	LoadSize;
end;

procedure TComponentStorage.LoadCollectionItemsProp(
	Component: TComponent; Collection: TCollection;
  PropName: string);
var
	i: Integer;
  s: string;
begin
	for i := 0 to Collection.Count - 1 do
  begin
    try
      s := ReadString(Component.Name + '_' +
      	Collection.ClassName + '_' + IntToStr(i) + '_' + PropName, '');
      SetPropValue(Collection.Items[i], PropName, s);
    except
    end;
  end;
end;

procedure TComponentStorage.SaveCollectionItemsProp(
	Component: TComponent; Collection: TCollection;
  PropName: string);
var
	i: Integer;
  s: string;
begin
	for i := 0 to Collection.Count - 1 do
  begin
    try
			s := GetPropValue(Collection.Items[i], PropName);
		  WriteString(Component.Name + '_' +
      	Collection.ClassName + '_' + IntToStr(i) + '_' + PropName, s);
    except
    end;
  end;
end;

{ TFormStorage }

constructor TFormStorage.Create(Form: TForm);
begin
  inherited Create(Form.ClassName, Form);
end;

procedure TFormStorage.LoadWindowState;
begin
  LoadPos;
  LoadProp('WindowState');
end;

procedure TFormStorage.SaveWindowState;
begin
  SaveProp('WindowState');
  if (FComponent is TForm) then
	  if TForm(FComponent).WindowState = wsNormal then
		  SavePos;
end;

end.



