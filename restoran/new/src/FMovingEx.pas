unit FMovingEx;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, MDIChildForm, DBCtrls, DBNavigatorEx, ExtCtrls, DB, ADODB,
  Grids, DBGrids, DBGridEx, ComCtrls, StdCtrls, Buttons, ToolWin, ActnList,
  ImgList, Menus, DBFilter, GuiUtils;

type
  TfrmMovingEx = class(TMDIChildForm)
    Grid: TDBGridEx;
    qryMoving: TADOQuery;
    dsMoving: TDataSource;
    pnlStorage: TPanel;
    DBNavigatorEx1: TDBNavigatorEx;
    calendarDateFrom: TDateTimePicker;
    calendarDateTo: TDateTimePicker;
    qryMovingName: TStringField;
    qryMovingStorage: TBCDField;
    qryMovingReceipt: TBCDField;
    qryMovingCharge: TBCDField;
    qryMovingDishCharge: TBCDField;
    qryMovingReceiptEnd: TBCDField;
    qryMovingChargeEnd: TBCDField;
    qryMovingDishChargeEnd: TBCDField;
    qryMovingQuantity: TBCDField;
    qryMovingStartQuantity: TBCDField;
    Images: TImageList;
    MovingActions: TActionList;
    acRefresh: TAction;
    acChange: TAction;
    ToolBar1: TToolBar;
    ToolButton1: TToolButton;
    ToolButton2: TToolButton;
    ToolButton3: TToolButton;
    MovingMenu: TMainMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    N3: TMenuItem;
    qryMovingId: TIntegerField;
    comboMeal: TComboBox;
    tblMeal: TADOTable;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure btnShowClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure qryMovingCalcFields(DataSet: TDataSet);
    procedure FormCreate(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure acChangeExecute(Sender: TObject);
    procedure comboMealChange(Sender: TObject);
  private
  	FCurrentDate: TDateTime;
  	FFilters: TFilters;
		FMealComboFilter: TComboFilter;
  public
  	property CurrentDate: TDateTime read FCurrentDate;
		procedure ShowMoving;
  end;

var
  frmMovingEx: TfrmMovingEx;

implementation

uses DMMain, Math, DateUtils, DMRestCalc, FCorrectRests;

{$R *.dfm}

procedure TfrmMovingEx.FormCreate(Sender: TObject);
begin
  FMealComboFilter := TComboFilter.Create(comboMeal,
  	tblMeal, 'Name', 'Id');

	// Create filters
	FFilters := TFilters.Create(TFilter);
	with FFilters.Add() as TFilter do FieldName := 'Id';

  FCommonStorage.LoadAllColumns;
end;

procedure TfrmMovingEx.FormDestroy(Sender: TObject);
begin
	FFilters.Free;
  FMealComboFilter.Free;
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmMovingEx.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
	Action := caFree;
end;

procedure TfrmMovingEx.btnShowClick(Sender: TObject);
begin
 	ShowMoving;
end;

procedure TfrmMovingEx.ShowMoving;
var
	MealName: string;
begin
	if Floor(calendarDateFrom.Date) > Floor(calendarDateTo.Date) then
  begin
  	MessageDlg('Конец периода не может быть меньше начала',
    	mtError, [mbOK], 0);
	  Exit;
  end;

  MealName := '';
  if qryMoving.Active then
  	MealName := qryMovingName.AsString;

	qryMoving.Close;
  FCurrentDate := Floor(calendarDateTo.Date);
  ConstructSelectMovingExSql(
  	qryMoving, Floor(calendarDateFrom.Date), FCurrentDate);
 	qryMoving.Open;
  if MealName <> '' then
  	qryMoving.Locate('Name', MealName, []);

  if Visible and Enabled then
	  Grid.SetFocus;

  SetCaption('Обороты - ' + DateToStr(calendarDateFrom.Date) +
  	' - ' + DateToStr(FCurrentDate));
end;

procedure TfrmMovingEx.qryMovingCalcFields(DataSet: TDataSet);
begin
  qryMovingStartQuantity.Value :=
    qryMovingStorage.Value +
    qryMovingReceipt.Value -
    qryMovingCharge.Value -
    qryMovingDishCharge.Value;

  qryMovingQuantity.Value :=
    qryMovingStartQuantity.Value +
    qryMovingReceiptEnd.Value -
    qryMovingChargeEnd.Value -
    qryMovingDishChargeEnd.Value;
end;

procedure TfrmMovingEx.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  	RefreshDataSets;

end;

procedure TfrmMovingEx.acChangeExecute(Sender: TObject);
var
	RetValue: Currency;
begin
	if GetCorrectRestValue(qryMovingName.AsString, FCurrentDate,
  	qryMovingQuantity.AsCurrency, RetValue) then
  begin
  	dmodRestCalc.CorrectRest(qryMovingId.AsInteger,
    	FCurrentDate, RetValue - qryMovingQuantity.AsCurrency);
    RefreshDataSets;
  end;
end;

procedure TfrmMovingEx.comboMealChange(Sender: TObject);
begin
  FMealComboFilter.AssignTo(FFilters.GetFilter('Id'));
  FFilters.ApplyFilters(qryMoving);
end;

end.
