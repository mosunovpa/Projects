unit FMovingPeriod;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls, MonthYearComboBox;

type
  TfrmMovingPeriod = class(TForm)
    btnOk: TButton;
    Button2: TButton;
    Sheet: TPageControl;
    PageMonth: TTabSheet;
    PagePeriod: TTabSheet;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    comboBeginMonth: TMonthYearComboBox;
    comboEndMonth: TMonthYearComboBox;
    Label4: TLabel;
    Label5: TLabel;
    comboMonth: TMonthYearComboBox;
    procedure FormCreate(Sender: TObject);
    procedure btnOkClick(Sender: TObject);
  private
    FBeginDate: TDateTime;
    FEndDate: TDateTime;
  public
    property BeginDate: TDateTime read FBeginDate write FBeginDate;
    property EndDate: TDateTime read FEndDate write FEndDate;
  end;

implementation

uses GuiUtils, DateUtils, DMRestCalc;

{$R *.dfm}

procedure TfrmMovingPeriod.FormCreate(Sender: TObject);
var
	BeginDate, EndDate: TDateTime;
  OperationExist: boolean;
begin
	OperationExist := dmodRestCalc.GetMaxMaxOperationDate(BeginDate, EndDate);
  if OperationExist = false then
  begin
    btnOk.Enabled := false;
    Exit;
  end;

  BeginDate := IncMonth(BeginDate);
  EndDate := IncMonth(EndDate);
  
	comboMonth.Fill(BeginDate, EndDate);
  comboMonth.Date := EndDate;

	comboBeginMonth.Fill(BeginDate, EndDate);
  comboBeginMonth.Date := EndDate;

	comboEndMonth.Fill(BeginDate, EndDate);
  comboEndMonth.Date := EndDate;

end;

procedure TfrmMovingPeriod.btnOkClick(Sender: TObject);
begin
	if Sheet.ActivePage = PageMonth then
  begin
	  FBeginDate := comboMonth.Date;
  	FEndDate := comboMonth.Date;
  end
  else
  begin
	  FBeginDate := comboBeginMonth.Date;
  	FEndDate := comboEndMonth.Date;
  end;

  if (FBeginDate > FEndDate) then
  begin
  	MessageDlg(' онец периода не может быть меньше начала',
    	mtError, [mbOK], 0);
	  ModalResult := mrNone;
  end;
end;

end.
