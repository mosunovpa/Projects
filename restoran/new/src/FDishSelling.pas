unit FDishSelling;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Storage, MDIChildForm, DBCtrls, ExtCtrls, DB, ADODB, Grids,
  DBGrids, DBNavigatorEx, DBGridEx, StdCtrls, DBFilter, GuiUtils;

const
	WM_UPDATE_SOLD_COMPS = WM_USER + 1;

  // set to True if you want filter DishSelling by MealId
  MEAL_FILTERED: Boolean = True;

type
  TfrmDishSelling = class(TMDIChildForm)
    Panel1: TPanel;
    gridDishes: TDBGridEx;
    tblDishSelling: TADOTable;
    dsDishSelling: TDataSource;
    tblDishSellingId: TAutoIncField;
    tblDishSellingDishId: TIntegerField;
    tblDishes: TADOTable;
    tblDishesId: TAutoIncField;
    tblDishesName: TWideStringField;
    tblDishesTypeId: TIntegerField;
    tblDishSellingDishName: TStringField;
    tblDishSellingSelDate: TDateTimeField;
    DBNavigatorEx1: TDBNavigatorEx;
    Splitter1: TSplitter;
    Panel2: TPanel;
    DBNavigatorEx2: TDBNavigatorEx;
    DBGridEx1: TDBGridEx;
    tblSoldDishComp: TADOTable;
    dsSoldDishComp: TDataSource;
    qry: TADOQuery;
    tblMeal: TADOTable;
    tblSoldDishCompSoldDishId: TIntegerField;
    tblSoldDishCompMealId: TIntegerField;
    tblSoldDishCompQuantity: TBCDField;
    tblSoldDishCompMealName: TStringField;
    tblDishSellingCount: TBCDField;
    comboDish: TComboBox;
    comboMeal: TComboBox;
    tblSoldCompAll: TADOTable;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure tblDishSellingAfterInsert(DataSet: TDataSet);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure tblDishSellingBeforePost(DataSet: TDataSet);
    procedure tblSoldDishCompBeforePost(DataSet: TDataSet);
    procedure tblDishSellingBeforeDelete(DataSet: TDataSet);
    procedure tblDishSellingAfterPost(DataSet: TDataSet);
    procedure comboDishChange(Sender: TObject);
    procedure tblDishSellingFilterRecord(DataSet: TDataSet;
      var Accept: Boolean);
    procedure comboMealChange(Sender: TObject);
    procedure tblSoldDishCompAfterPost(DataSet: TDataSet);
    procedure tblSoldDishCompBeforeDelete(DataSet: TDataSet);
  protected
  	FDishSellingState: TDataSetState;
    procedure OnUpdateSoldComp(var Message: TMessage); message WM_UPDATE_SOLD_COMPS;
  private
  	FSoldMealChanged: Boolean;
   	FFilters: TFilters;
		FMealComboFilter: TComboFilter;
		FDishComboFilter: TComboFilter;
		FMealId: Integer;
    procedure SetSoldMealChanged(Value: Boolean);
  	procedure InsertDishComponents;
    procedure DeleteDishComponents;
  public
		property SoldMealChanged: Boolean read FSoldMealChanged write SetSoldMealChanged;
  end;

implementation

uses DMMain, DBUtils;

{$R *.dfm}

procedure TfrmDishSelling.FormCreate(Sender: TObject);
begin
	FDishComboFilter := TComboFilter.Create(comboDish,
  	tblDishes, 'Name', 'Id');
  if (MEAL_FILTERED = True) then
		FMealComboFilter := TComboFilter.Create(comboMeal, tblMeal, 'Name', 'Id')
  else
		comboMeal.Visible := False;

 	FFilters := TFilters.Create(TFilter);
  with FFilters.Add() as TFilter do FieldName := 'DishId';

 	OpenBigDataSet(tblDishes);
 	OpenBigDataSet(tblDishSelling, 'SelDate DESC');
 	OpenBigDataSet(tblSoldDishComp);
  if (MEAL_FILTERED = True) then
	  OpenBigDataSet(tblSoldCompAll);

	FCommonStorage.LoadComponentProp(gridDishes, 'Height');
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmDishSelling.FormDestroy(Sender: TObject);
begin
  if (MEAL_FILTERED = True) then
		FMealComboFilter.Free;

  FDishComboFilter.Free;
	FFilters.Free;

	FCommonStorage.SaveComponentProp(gridDishes, 'Height');
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmDishSelling.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Action := caFree;
end;


procedure TfrmDishSelling.tblDishSellingAfterInsert(DataSet: TDataSet);
begin
  tblDishSellingSelDate.NewValue := dmodMain.CurDate;
  FFilters.SetFilteredValuesTo(tblDishSelling);
end;

procedure TfrmDishSelling.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  	RefreshDataSets;

end;

procedure TfrmDishSelling.tblDishSellingBeforePost(DataSet: TDataSet);
begin
	CheckNullField(TCustomADODataSet(DataSet));
  if DataSet.State = dsEdit then
  	if tblDishSellingDishId.OldValue <> tblDishSellingDishId.Value then
    begin
    	MessageDlg('«апрещено редактировать уже проданное блюдо',
      	mtError, [mbOK], 0);
      Abort;
    end;
  CheckRecFiltered(FFilters, TCustomADODataSet(DataSet));
  FDishSellingState := DataSet.State;
end;

procedure TfrmDishSelling.tblDishSellingAfterPost(DataSet: TDataSet);
var
	Filtered: Boolean;
begin
	Filtered := False;
  if (FDishSellingState = dsInsert) then
  	try
		  Filtered := DataSet.Filtered;
		  DataSet.Filtered := False;
    	// insert components from DishComponents
	  	InsertDishComponents;
      OpenBigDataSet(tblSoldDishComp);
      DataSet.Filtered := Filtered;
    except
    	on e: Exception do
      begin
      	tblDishSelling.Delete;  // on error delete sold dish
      	Application.ShowException(e);
	      DataSet.Filtered := Filtered;
	      Abort;
      end;
    end;
end;

procedure TfrmDishSelling.InsertDishComponents;
var
	s: string;
begin
	with qry do
  begin
  	s :=
    	'insert into SoldDishComponents (SoldDishId, MealId, Quantity) ' +
      'select %d, MealId, Quantity from DishComponents ' +
      'where DishId = %d';
	  qry.Close;
  	qry.SQL.Clear;
    qry.SQL.Add(Format(s,
    	[tblDishSellingId.AsInteger, tblDishSellingDishId.AsInteger]));
    qry.ExecSQL;
    qry.Close;
	end;
  
	SoldMealChanged := True;
end;

procedure TfrmDishSelling.tblDishSellingBeforeDelete(DataSet: TDataSet);
begin
	DeleteDishComponents;
end;

procedure TfrmDishSelling.DeleteDishComponents;
var
	s: string;
begin
	with qry do
  begin
  	s :=
    	'delete from SoldDishComponents  ' +
      'where SoldDishId = %d';
	  qry.Close;
  	qry.SQL.Clear;
    qry.SQL.Add(Format(s, [tblDishSellingId.AsInteger]));
    qry.ExecSQL;
    qry.Close;
	end;

  SoldMealChanged := True;
end;

procedure TfrmDishSelling.tblSoldDishCompBeforePost(DataSet: TDataSet);
begin
	CheckNullField(TCustomADODataSet(DataSet));
end;

procedure TfrmDishSelling.comboMealChange(Sender: TObject);
var
	Save_Cursor: TCursor;
begin
	if MEAL_FILTERED = False then
  	Exit;

  // clear Dish filter
  if (comboDish.ItemIndex <> 0) then
  begin
    comboDish.ItemIndex := 0;
    tblDishSelling.Filter := '';
  end;

  if (FSoldMealChanged = False)
  	and (FMealId = Integer(comboMeal.Items.Objects[comboMeal.ItemIndex])) then
  	Exit;

	Update;
    
  FMealId := Integer(comboMeal.Items.Objects[comboMeal.ItemIndex]);

  if FSoldMealChanged = True then
  begin
    OpenBigDataSet(tblSoldCompAll);
    FSoldMealChanged := False;
  end;

  tblDishSelling.Filtered := False;

	if FMealId > 0 then // needed filter by Meal
  begin
	  // set filter by Meal (see tblDishSellingFilterRecord())
    Save_Cursor := Screen.Cursor;
    try
      Screen.Cursor := crHourGlass;

			tblDishSelling.Filtered := True;
//			SendMessage(Handle, WM_UPDATE_SOLD_COMPS, 0, 0);
    finally
      Screen.Cursor := Save_Cursor;
    end;
  end;
end;

procedure TfrmDishSelling.comboDishChange(Sender: TObject);
begin
	// clear meal filter if it was set
	if (MEAL_FILTERED = True) and (comboMeal.ItemIndex <> 0) then
  begin
	  FMealId := 0; // clear meal filter
  	comboMeal.ItemIndex := 0;
  end;

  FDishComboFilter.AssignTo(FFilters.GetFilter('DishId'));
  FFilters.ApplyFilters(tblDishSelling);
end;

procedure TfrmDishSelling.tblDishSellingFilterRecord(DataSet: TDataSet;
  var Accept: Boolean);
//var
//	s: string;
begin
	Accept := True;
  if (MEAL_FILTERED = True) and (FMealId <> 0) then
  begin
  	Accept := tblSoldCompAll.Locate('MealId;SoldDishId',
    	VarArrayOf([FMealId, DataSet.Fields[0].AsInteger]), []);
//    Accept := tblSoldCompAll.Seek(VarArrayOf([FMealId, DataSet.Fields[0].AsInteger]), soFirstEQ);
  {
    with qry do
    begin
      s :=
        'select MealId from SoldDishComponents  ' +
        'where SoldDishId = %d and MealId = %d';
      qry.Close;
      qry.SQL.Clear;
      qry.SQL.Add(Format(s,
        [DataSet.Fields[0].AsInteger, FMealId]));
      qry.Open;
      if qry.Eof and qry.Bof then
				Accept := False;
      qry.Close;
    end;
    }
  end;
end;

procedure TfrmDishSelling.tblSoldDishCompAfterPost(DataSet: TDataSet);
begin
  SoldMealChanged := True;
end;

procedure TfrmDishSelling.tblSoldDishCompBeforeDelete(DataSet: TDataSet);
begin
  SoldMealChanged := True;
end;

procedure TfrmDishSelling.OnUpdateSoldComp(var Message: TMessage);
var
	Save_Cursor: TCursor;
begin
	if (MEAL_FILTERED = True) and (FSoldMealChanged = True) then
  begin
    Save_Cursor := Screen.Cursor;
    try
      Screen.Cursor := crHourGlass;
      OpenBigDataSet(tblSoldCompAll);
      FSoldMealChanged := False;
      // refresh tblDishSelling filter range
      if FMealId > 0 then
      begin
        tblDishSelling.Filtered := False;
        tblDishSelling.Filtered := True;
      end;
    finally
      Screen.Cursor := Save_Cursor;
    end;
  end;
end;

procedure TfrmDishSelling.SetSoldMealChanged(Value: Boolean);
begin
  FSoldMealChanged := Value;
  if FSoldMealChanged = True then
  begin
//		PostMessage(Handle, WM_UPDATE_SOLD_COMPS, 0, 0);
  end;
end;

end.
