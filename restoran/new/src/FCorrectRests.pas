unit FCorrectRests;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

function GetCorrectRestValue(ProdName: string; RestDate: TDateTime;
	OldValue: Currency; var NewValue: Currency): Boolean;

type
  TfrmCorrectRests = class(TForm)
    lblTitle: TLabel;
    Label2: TLabel;
    editCurrentRests: TEdit;
    editNewRests: TEdit;
    Label3: TLabel;
    btnCancel: TButton;
    btnOk: TButton;
    procedure btnOkClick(Sender: TObject);
  private
    FNewRest: Currency;
  public
    property NewRest: Currency read FNewRest write FNewRest;
  end;

implementation

{$R *.dfm}

function GetCorrectRestValue(ProdName: string; RestDate: TDateTime;
	OldValue: Currency; var NewValue: Currency): Boolean;
begin
  Result := False;
  with TfrmCorrectRests.Create(Application) do
  	try
    	lblTitle.Caption := Format(lblTitle.Caption,
      	[DateToStr(RestDate), ProdName]);
      editCurrentRests.Text := CurrToStr(OldValue);
			if ShowModal = mrOk then
      begin
        NewValue := NewRest;
      	Result := True;
      end;
    finally
    	Free;
    end;
end;

procedure TfrmCorrectRests.btnOkClick(Sender: TObject);
begin
	try
  	NewRest := StrToCurr(editNewRests.Text);
  except
  	MessageDlg('Неверное значение новых остатков.',
	    mtError, [mbOK], 0);
    ModalResult := mrNone;
  end;
end;

end.
