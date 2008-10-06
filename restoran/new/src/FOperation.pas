unit FOperation;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Storage, MDIChildForm, StdCtrls, DBCtrls, Grids, DBGrids,
  ExtCtrls, DB, ADODB, DBNavigatorEx, DBGridEx, DMRestCalc, DBFilter,
  GuiUtils, OperationFilter;

type
  TfrmOperations = class(TMDIChildForm)
    Panel1: TPanel;
    DBGrid: TDBGridEx;
    comboOperations: TComboBox;
    tblOperations: TADOTable;
    tblMeal: TADOTable;
    tblOperationTypes: TADOTable;
    dsOperations: TDataSource;
    tblOperationsId: TAutoIncField;
    tblOperationsOperationDate: TDateTimeField;
    tblOperationsMealId: TIntegerField;
    tblOperationsQuantity: TBCDField;
    tblOperationsOperationId: TWordField;
    tblOperationsMealName: TStringField;
    tblOperationsOperationName: TStringField;
    tblOperationTypesId: TWordField;
    tblOperationTypesName: TWideStringField;
    DBNavigatorEx1: TDBNavigatorEx;
    comboMeal: TComboBox;
    tblOperationsType: TIntegerField;
    tblOperationSubTypes: TADOTable;
    tblOperationsSubType: TStringField;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure comboOperationsChange(Sender: TObject);
    procedure tblOperationsAfterInsert(DataSet: TDataSet);
    procedure tblOperationsBeforePost(DataSet: TDataSet);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure comboMealChange(Sender: TObject);
    procedure tblOperationsAfterPost(DataSet: TDataSet);
  private
  	FFilters: TFilters;
		FMealComboFilter: TComboFilter;
  	FOpComboFilter: TOperationFilter;
    FPrevOperOperId: Integer;
  public
    { Public declarations }
  end;

implementation

uses DMMain, DBUtils, StrUtil, Constants;

{$R *.dfm}

procedure TfrmOperations.FormCreate(Sender: TObject);
begin
  // Poplate comboboxes
 	FOpComboFilter := TOperationFilter.Create(comboOperations);
  FMealComboFilter := TComboFilter.Create(comboMeal,
  	tblMeal, 'Name', 'Id');

	// Create filters
	FFilters := TFilters.Create(TFilter);
  with FFilters.Add() as TFilter do FieldName := 'OperationId';
  with FFilters.Add() as TFilter do FieldName := 'Type';
	with FFilters.Add() as TFilter do FieldName := 'MealId';

  OpenBigDataSet(tblOperations, 'OperationDate DESC');
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmOperations.FormDestroy(Sender: TObject);
begin
	FFilters.Free;
  FMealComboFilter.Free;
  FOpComboFilter.Free;
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmOperations.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
  Action := caFree;
end;

procedure TfrmOperations.comboOperationsChange(Sender: TObject);
begin
  FOpComboFilter.AssignTo(FFilters.GetFilter('OperationId'),
  	FFilters.GetFilter('Type'));
  FFilters.ApplyFilters(tblOperations);
end;

procedure TfrmOperations.comboMealChange(Sender: TObject);
begin
  FMealComboFilter.AssignTo(FFilters.GetFilter('MealId'));
  FFilters.ApplyFilters(tblOperations);
end;

procedure TfrmOperations.tblOperationsAfterInsert(DataSet: TDataSet);
begin
	tblOperationsOperationDate.NewValue := dmodMain.CurDate;

  FFilters.SetFilteredValuesTo(tblOperations);

  if FFilters.GetFilter('Type').Active = False then
 	  tblOperationsType.NewValue := SUBTYPE_NONE;

  if FFilters.GetFilter('OperationId').Active = False then
 	  tblOperationsOperationId.NewValue := FPrevOperOperId;

end;

procedure TfrmOperations.tblOperationsBeforePost(DataSet: TDataSet);
begin
	CheckNullField(TCustomADODataSet(DataSet));
  CheckRecFiltered(FFilters, TCustomADODataSet(DataSet));
end;

procedure TfrmOperations.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
			RefreshDataSets;
  if (Key = VK_SPACE) and
  	(DBGrid.SelectedField = tblOperationsOperationName) then
  begin
  	if tblOperations.State = dsBrowse then
	    tblOperations.Edit();

    tblOperationsOperationId.AsInteger :=
		    (tblOperationsOperationId.AsInteger mod 2) + 1;
  end;
end;

procedure TfrmOperations.tblOperationsAfterPost(DataSet: TDataSet);
begin
  FPrevOperOperId := tblOperationsOperationId.AsInteger;
end;

end.
