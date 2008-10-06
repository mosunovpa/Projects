unit FLogin;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, DB, ADODB;

type
  TfrmLogin = class(TForm)
    editName: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    editPassword: TEdit;
    btnOk: TButton;
    Button2: TButton;
    qryLogin: TADOQuery;
    procedure btnOkClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

implementation

uses DMMain;

{$R *.dfm}

procedure TfrmLogin.btnOkClick(Sender: TObject);
begin
	with qryLogin do
  begin
  	Close;
    Parameters[0].Value := editName.Text;
    Parameters[1].Value := editPassword.Text;
    try
			Open;
      if Eof then
      	raise Exception.Create('Неверно указаны имя или пароль.');
    except
    	on e: Exception do
      begin
      	Application.ShowException(e);
	      ModalResult := mrNone;
      end;
    end;
  end;

end;

end.
