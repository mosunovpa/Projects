unit FCount;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls;

type
  TfrmCount = class(TForm)
    editCount: TEdit;
    Label1: TLabel;
    Button1: TButton;
    Button2: TButton;
    Bevel1: TBevel;
    procedure Button1Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    Count: integer;
  end;

var
  frmCount: TfrmCount;

implementation

{$R *.DFM}

procedure TfrmCount.Button1Click(Sender: TObject);
var
	s: string;
begin
     try
     	s := Trim(editCount.Text);
        if s = '' then
        	Count := 0
        else
	        Count := StrToInt(s);
     except
        ShowMessage('Неверное количество');
        ModalResult := 0;
     end;
end;

procedure TfrmCount.FormShow(Sender: TObject);
begin
	editCount.Text := IntToStr(Count);
	editCount.SetFocus;
end;

end.
