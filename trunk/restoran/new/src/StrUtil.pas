unit StrUtil;

interface

uses
	Classes;

type
  TFilterFormatter = class
  public
  	constructor Create();
    destructor Destroy(); override;
    procedure AddFilter(s: string);
    function GetString(): string;
  protected
    FList: TStringList;
  end;

implementation

{ TFilterFormatter }

constructor TFilterFormatter.Create;
begin
  FList := TStringList.Create;
end;

destructor TFilterFormatter.Destroy;
begin
  FList.Free;
  inherited;
end;

procedure TFilterFormatter.AddFilter(s: string);
begin
  FList.Add(s);
end;

function TFilterFormatter.GetString: string;
var
	i: Integer;
begin
	Result := '';
	for i := 0 to FList.Count - 1 do
  begin
		if Result <> '' then
    	Result := Result + ' and ';

    Result := Result + FList[i];
  end;
end;

end.
