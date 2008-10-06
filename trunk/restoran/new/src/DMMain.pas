unit DMMain;

interface

uses
  SysUtils, Classes, DB, ADODB, ActnList, Controls, Math;

type
  TdmodMain = class(TDataModule)
    connDB: TADOConnection;
    procedure DataModuleCreate(Sender: TObject);
    procedure DataModuleDestroy(Sender: TObject);
  private
    FDate: TDateTime;
    function GetDate: TDateTime;
    procedure SetDate(const Value: TDateTime);
  public
    property CurDate: TDateTime read GetDate write SetDate;
  end;

var
  dmodMain: TdmodMain;

implementation

{$R *.dfm}

uses
	Forms, FLogin;

procedure TdmodMain.DataModuleCreate(Sender: TObject);
var
	FileDir: string;
begin
	FileDir := ExtractFileDir(Application.ExeName);
  connDB.Connected := false;
	connDB.ConnectionString := Format(
		'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\data\restoran.mdb;Persist Security Info=False',
    [FileDir]);
  connDB.Connected := true;
{
	with TfrmLogin.Create(Application) do
    try
      if ShowModal = mrCancel then
      	Application.Terminate;
    finally
    	Free;
    end;
}

  CurDate := Date;
end;

procedure TdmodMain.DataModuleDestroy(Sender: TObject);
begin
  connDB.Connected := false;
end;

function TdmodMain.GetDate: TDateTime;
begin
	Result := FDate
end;

procedure TdmodMain.SetDate(const Value: TDateTime);
begin
  FDate := Floor(Value);
end;

end.
