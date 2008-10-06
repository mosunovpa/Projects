unit FDishesTypes;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, ActnList, Grids, DBGrids, DBCtrls, ExtCtrls, DB, ADODB,
  MDIChildForm, DBNavigatorEx, DBGridEx;

type
  TfrmDishesTypes = class(TMDIChildForm)
    Panel1: TPanel;
    DBGrid1: TDBGridEx;
    tblDishesTypes: TADOTable;
    dsDishesTypes: TDataSource;
    tblDishesTypesId: TAutoIncField;
    tblDishesTypesName: TWideStringField;
    navDishesTypes: TDBNavigatorEx;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure tblDishesTypesDeleteError(DataSet: TDataSet;
      E: EDatabaseError; var Action: TDataAction);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure tblDishesTypesBeforePost(DataSet: TDataSet);
  private
  public
    { Public declarations }
  end;

implementation

uses DMMain, DBUtils;

{$R *.dfm}

procedure TfrmDishesTypes.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
	Action := caFree;
end;

procedure TfrmDishesTypes.FormCreate(Sender: TObject);
begin
	OpenBigDataSet(tblDishesTypes, 'Name');
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmDishesTypes.FormDestroy(Sender: TObject);
begin
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmDishesTypes.tblDishesTypesDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);
begin
	ProcessDeleteError(DataSet, E, Action);
end;

procedure TfrmDishesTypes.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  	RefreshDataSets;
end;

procedure TfrmDishesTypes.tblDishesTypesBeforePost(DataSet: TDataSet);
begin
	CheckNullField(TCustomADODataSet(DataSet));
end;

end.
