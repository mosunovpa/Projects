unit FMealTypes;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Grids, DBGrids, DBCtrls, ExtCtrls, DB, ActnList, Menus, ADODB,
  MDIChildForm, DBNavigatorEx, DBGridEx;

type
  TfrmMealTypes = class(TMDIChildForm)
    Panel1: TPanel;
    DBGrid1: TDBGridEx;
    tblMealTypes: TADOTable;
    dsMealTypes: TDataSource;
    tblMealTypesId: TAutoIncField;
    tblMealTypesName: TWideStringField;
    navMealTypes: TDBNavigatorEx;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure tblMealTypesDeleteError(DataSet: TDataSet; E: EDatabaseError;
      var Action: TDataAction);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure tblMealTypesBeforePost(DataSet: TDataSet);
  private
  public
    { Public declarations }
  end;

implementation

uses DMMain, DBUtils, TypInfo;

{$R *.dfm}

procedure TfrmMealTypes.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
	Action := caFree;
end;

procedure TfrmMealTypes.FormCreate(Sender: TObject);
begin
	OpenBigDataSet(tblMealTypes, 'Name');
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmMealTypes.FormDestroy(Sender: TObject);
begin
  FCommonStorage.SaveAllColumns;
end;


procedure TfrmMealTypes.tblMealTypesDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);
begin
	ProcessDeleteError(DataSet, E, Action);
end;

procedure TfrmMealTypes.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  	RefreshDataSets;

end;

procedure TfrmMealTypes.tblMealTypesBeforePost(DataSet: TDataSet);
begin
	CheckNullField(TCustomADODataSet(DataSet));

end;

end.
