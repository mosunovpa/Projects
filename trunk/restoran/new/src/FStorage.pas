unit FStorage;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Storage, MDIChildForm, DB, ADODB, Grids, DBGrids, DBCtrls,
  ExtCtrls, DBNavigatorEx, ComCtrls, StdCtrls, DateUtils, DBGridEx,
  MonthYearComboBox, DBFilter, GuiUtils;

type
  TfrmStorage = class(TMDIChildForm)
    pnlStorage: TPanel;
    DBGrid1: TDBGridEx;
    qryStorage: TADOQuery;
    dsStorage: TDataSource;
    DBNavigatorEx1: TDBNavigatorEx;
    comboMonth: TMonthYearComboBox;
    qryStorageDate: TDateTimeField;
    qryStorageName: TWideStringField;
    qryStorageQuantity: TBCDField;
    comboMeal: TComboBox;
    tblMeal: TADOTable;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure btnShowClick(Sender: TObject);
    procedure comboMonthChange(Sender: TObject);
    procedure DBNavigatorEx1BeforeAction(Sender: TObject;
      Button: TNavigateBtn);
    procedure FormDestroy(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure comboMealChange(Sender: TObject);
  private
  	FFilters: TFilters;
		FMealComboFilter: TComboFilter;
    procedure FillMonthCombo;
  public
	  class function IsCanCreate: boolean; override;
  end;

implementation

uses
	DMRestCalc, Constants, DMMain;

{$R *.dfm}

procedure TfrmStorage.FormCreate(Sender: TObject);
begin
  FMealComboFilter := TComboFilter.Create(comboMeal,
  	tblMeal, 'Name', 'Id');

	// Create filters
	FFilters := TFilters.Create(TFilter);
	with FFilters.Add() as TFilter do FieldName := 'Id';

  FillMonthCombo;
 	comboMonthChange(comboMonth);
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmStorage.FormDestroy(Sender: TObject);
begin
	FFilters.Free;
  FMealComboFilter.Free;
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmStorage.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Action := caFree;
end;

class function TfrmStorage.IsCanCreate: boolean;
begin
	Result := true;
end;

procedure TfrmStorage.btnShowClick(Sender: TObject);
var
  MealName: string;
begin
	MealName := '';
	if qryStorage.Active then
  	MealName := qryStorageName.AsString;

  qryStorage.Close;
  qryStorage.Parameters[0].Value :=
  	comboMonth.Date;

  qryStorage.Open;
  if MealName <> '' then
  	qryStorage.Locate('Name', MealName, []);
end;

procedure TfrmStorage.comboMonthChange(Sender: TObject);
begin
  btnShowClick(Sender);
  SetCaption('Остатки - ' + comboMonth.Text);
end;

procedure TfrmStorage.FillMonthCombo;
var
	CurDate, BeginDate, EndDate: TDateTime;
  StorageExist: boolean;
begin
	StorageExist := dmodRestCalc.GetMinMaxStorageDate(BeginDate, EndDate);
  if StorageExist = false then
  begin
  	BeginDate := Date;
  	EndDate := Date;
  end;

  CurDate := comboMonth.Date;
	if CurDate = 0 then
  	CurDate:= EndDate;

	if (CurDate < BeginDate) or (CurDate > EndDate) then
	begin
		MessageDlg('Остатки на "' + DateToStr(CurDate) + '" отсутствуют.',
    		mtInformation, [mbOk], 0);
		CurDate := EndDate;
	end;
	comboMonth.Fill(BeginDate, EndDate);
	comboMonth.Date := CurDate;
end;

procedure TfrmStorage.DBNavigatorEx1BeforeAction(Sender: TObject;
  Button: TNavigateBtn);
begin
	if Button = nbRefresh then
	  FillMonthCombo;
end;

procedure TfrmStorage.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  begin
	  FillMonthCombo;
  	RefreshDataSets;
  end;

end;

procedure TfrmStorage.comboMealChange(Sender: TObject);
begin
  FMealComboFilter.AssignTo(FFilters.GetFilter('Id'));
  FFilters.ApplyFilters(qryStorage);
end;

end.
