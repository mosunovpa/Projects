unit FSetMovingDate;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls;

type
  TfrmSetMovingDate = class(TForm)
    Sheet: TPageControl;
    PageDate: TTabSheet;
    PagePeriod: TTabSheet;
    Button1: TButton;
    Button2: TButton;
    Label1: TLabel;
    calendarDate: TDateTimePicker;
    Label2: TLabel;
    calendarFrom: TDateTimePicker;
    calendarTo: TDateTimePicker;
    Label3: TLabel;
    Label4: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSetMovingDate: TfrmSetMovingDate;

implementation

uses
	DateUtils, Math;

{$R *.dfm}

procedure TfrmSetMovingDate.FormCreate(Sender: TObject);
begin
	calendarDate.Date := Date;
  calendarFrom.Date := StartOfTheMonth(Date);
	calendarTo.Date := Date;
end;

procedure TfrmSetMovingDate.Button1Click(Sender: TObject);
begin
  if (Sheet.ActivePage = PagePeriod) and
  	(Floor(calendarFrom.Date) > Floor(calendarTo.Date)) then
  begin
  	MessageDlg(' онец периода не может быть меньше начала',
    	mtError, [mbOK], 0);
	  ModalResult := mrNone;
  end;
end;

end.
