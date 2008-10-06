unit FSetDate;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, StdCtrls, ExtCtrls;

type
  TfrmSetDate = class(TForm)
    calendar: TDateTimePicker;
    lblTitle: TLabel;
  published
    Button1: TButton;
    Button2: TButton;
    procedure calendarDblClick(Sender: TObject);
  private
  private
    { Private declarations }
  public
  end;

implementation

{$R *.dfm}

procedure TfrmSetDate.calendarDblClick(Sender: TObject);
begin
	ModalResult := mrOk;
end;

end.
