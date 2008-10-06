unit FMealSelling;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, MDIChildForm, Storage, DB, ADODB, Grids, DBGrids, StdCtrls,
  DBCtrls, DBNavigatorEx, ExtCtrls, DBFilter, GuiUtils;

type
  TfrmMealSelling = class(TMDIChildForm)
    Panel1: TPanel;
    DBNavigatorEx1: TDBNavigatorEx;
    comboMeal: TComboBox;
    DBGrid1: TDBGrid;
    dsMealSelling: TDataSource;
    qryMealSelling: TADOQuery;
    tblMeal: TADOTable;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure comboMealChange(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
  private
   	FFilters: TFilters;
		FMealComboFilter: TComboFilter;
  public
    { Public declarations }
  end;

implementation

uses DMMain, DBUtils;

{$R *.dfm}

procedure TfrmMealSelling.FormCreate(Sender: TObject);
begin
	FMealComboFilter := TComboFilter.Create(comboMeal, tblMeal, 'Name', 'Id');
 	FFilters := TFilters.Create(TFilter);
  with FFilters.Add() as TFilter do FieldName := 'MealId';
 	OpenBigDataSet(qryMealSelling);
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmMealSelling.FormDestroy(Sender: TObject);
begin
  FCommonStorage.SaveAllColumns;
	FMealComboFilter.Free;
	FFilters.Free;
end;

procedure TfrmMealSelling.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
  Action := caFree;
end;

procedure TfrmMealSelling.comboMealChange(Sender: TObject);
begin
  FMealComboFilter.AssignTo(FFilters.GetFilter('MealId'));
  FFilters.ApplyFilters(qryMealSelling);
end;

procedure TfrmMealSelling.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  	RefreshDataSets;
end;

end.
