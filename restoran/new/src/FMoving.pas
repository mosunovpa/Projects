unit FMoving;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, MDIChildForm, Storage, DB, ADODB, Grids, DBGrids, DBCtrls,
  DBNavigatorEx, ExtCtrls, DBGridEx, ComCtrls, StdCtrls, Buttons, ImgList,
  ActnList, ToolWin, Menus, DBFilter, GuiUtils;

type
  TfrmMoving = class(TMDIChildForm)
    pnlMoving: TPanel;
    DBNavigatorEx1: TDBNavigatorEx;
    Grid: TDBGridEx;
    dsMoving: TDataSource;
    qryMoving: TADOQuery;
    calendarDate: TDateTimePicker;
    qryMovingName: TStringField;
    qryMovingStorage: TBCDField;
    qryMovingReceipt: TBCDField;
    qryMovingCharge: TBCDField;
    qryMovingDishCharge: TBCDField;
    qryMovingQuantity: TBCDField;
    MovingActions: TActionList;
    acRefresh: TAction;
    acChange: TAction;
    Images: TImageList;
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

implementation

uses DMMain, DMRestCalc, DateUtils, Math, DBUtils, FCorrectRests;

{$R *.dfm}

procedure TfrmMoving.FormCreate(Sender: TObject);
begin
  FMealComboFilter := TComboFilter.Create(comboMeal,
  	tblMeal, 'Name', 'Id');

	// Create filters
	FFilters := TFilters.Create(TFilter);
	with FFilters.Add() as TFilter do FieldName := 'Id';
  
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmMoving.FormDestroy(Sender: TObject);
begin
	FFilters.Free;
  FMealComboFilter.Free;
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmMoving.FormClose(Sender: TObject; var Action: TCloseAction);
begin
	Action := caFree;
end;


procedure TfrmMoving.btnShowClick(Sender: TObject);
begin
 	ShowMoving;
end;

procedure TfrmMoving.ShowMoving;
var
	MealName: string;
begin
  MealName := '';
  if qryMoving.Active then
  	MealName := qryMovingName.AsString;

	qryMoving.Close;
  FCurrentDate := Floor(calendarDate.Date);
  ConstructSelectMovingSql(qryMoving, FCurrentDate);
 	qryMoving.Open;
  if MealName <> '' then
  	qryMoving.Locate('Name', MealName, []);

  if Visible and Enabled then
	  Grid.SetFocus;

  SetCaption('Обороты - ' + DateToStr(FCurrentDate));
end;

procedure TfrmMoving.qryMovingCalcFields(DataSet: TDataSet);
begin
  qryMovingQuantity.Value :=
    qryMovingStorage.Value +
    qryMovingReceipt.Value -
    qryMovingCharge.Value -
    qryMovingDishCharge.Value;

end;

procedure TfrmMoving.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  	RefreshDataSets;

end;

procedure TfrmMoving.acChangeExecute(Sender: TObject);
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

procedure TfrmMoving.comboMealChange(Sender: TObject);
begin
  FMealComboFilter.AssignTo(FFilters.GetFilter('Id'));
  FFilters.ApplyFilters(qryMoving);
end;

end.
