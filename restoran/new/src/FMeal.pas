unit FMeal;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Grids, DBGrids, ExtCtrls, StdCtrls, DBCtrls, DB, ADODB, ComCtrls,
  ActnList, Menus, MDIChildForm, Storage, DBNavigatorEx, DBGridEx;

type
  TfrmMeal = class(TMDIChildForm)
    Splitter1: TSplitter;
    tblTypes: TADOTable;
    tblMeal: TADOTable;
    dsMeal: TDataSource;
    tblMealId: TAutoIncField;
    tblMealName: TWideStringField;
    tblMealTypeId: TIntegerField;
    tblMealTypeName: TStringField;
    tblTypesId: TAutoIncField;
    tblTypesName: TWideStringField;
    Panel2: TPanel;
    DBGrid1: TDBGridEx;
    pnlMeal: TPanel;
    pnlCategory: TPanel;
    tvTypes: TTreeView;
    Panel4: TPanel;
    DBNavigatorEx1: TDBNavigatorEx;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure tvTypesChange(Sender: TObject; Node: TTreeNode);
    procedure tblMealBeforePost(DataSet: TDataSet);
    procedure tblMealAfterInsert(DataSet: TDataSet);
    procedure tblMealDeleteError(DataSet: TDataSet; E: EDatabaseError;
      var Action: TDataAction);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure DBNavigatorEx1BeforeAction(Sender: TObject;
      Button: TNavigateBtn);
  private
    FTypeId: Integer;
    procedure FillTypesTree;
  public
    { Public declarations }
  end;

implementation

uses DMMain, DBUtils;

{$R *.dfm}

resourcestring
	rsMealCaption = 'Продукты';

{ TfrmMeal }

procedure TfrmMeal.FillTypesTree;
var
	RootNode: TTreeNode;
	Node: TTreeNode;
	SelNode: Integer;
begin
	// store current selection
	SelNode := 0;
	if tvTypes.Selected <> nil then
		SelNode := Integer(tvTypes.Selected.Data);

	tvTypes.Items.Clear;
  // add first node
	RootNode := tvTypes.Items.AddObject(nil, 'Продукты', Pointer(0));

	// populate tree view
  tblTypes.Close;
  OpenBigDataSet(tblTypes, 'Name');
	tblTypes.First;
  while not tblTypes.Eof do
  begin
  	// add node
	  Node := tvTypes.Items.AddChildObject(RootNode,
   		tblTypesName.AsString,
      Pointer(tblTypesId.AsInteger));

    // restore selection
    if SelNode = tblTypesId.AsInteger then
      tvTypes.Selected := Node;

    tblTypes.Next;
  end;
  RootNode.Expand(false);
  // if no selection - select first node
	if tvTypes.Selected = nil then
	  tvTypes.Selected := RootNode;

  tblTypes.Close;
end;

procedure TfrmMeal.FormClose(Sender: TObject; var Action: TCloseAction);
begin
	Action := caFree;
end;

procedure TfrmMeal.FormCreate(Sender: TObject);
begin
  FillTypesTree;
  OpenBigDataSet(tblMeal, 'Name');
	FCommonStorage.LoadComponentProp(pnlCategory, 'Width');
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmMeal.FormDestroy(Sender: TObject);
begin
	FCommonStorage.SaveComponentProp(pnlCategory, 'Width');
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmMeal.tvTypesChange(Sender: TObject; Node: TTreeNode);
begin
	FTypeId := Integer(Node.Data);
	if FTypeId = 0 then
  begin
    tblMeal.Filtered := false;
  end
  else if FTypeId > 0 then
  begin
	  tblMeal.Filter := 'TypeId = ' + IntToStr(FTypeId);
  	tblMeal.Filtered := true;
  end;
  if tblMeal.Filtered = false then
  	pnlMeal.Caption := Format('%s:', [rsMealCaption])
	else
	  pnlMeal.Caption := Format('%s (%s):', [rsMealCaption, Node.Text]);
end;

procedure TfrmMeal.tblMealBeforePost(DataSet: TDataSet);
begin
	CheckNullField(TCustomADODataSet(DataSet));
	if (tblMeal.Filtered = true)
  	and (tblMealTypeId.Value <> FTypeId)
    then
  begin
  	if mrCancel = MessageDlg(
    	'Продукт будет перемещен в категорию "' +
      tblTypesName.AsString + '".' + #13#10 +
      'Сохранить изменения?',
    	mtInformation, [mbOK, mbCancel], 0) then
    begin
      Abort;
    end;
  end;
end;

procedure TfrmMeal.tblMealAfterInsert(DataSet: TDataSet);
begin
	if tblMeal.Filtered = true then
  begin
    tblMealTypeId.NewValue := FTypeId;
  end;
end;

procedure TfrmMeal.tblMealDeleteError(DataSet: TDataSet; E: EDatabaseError;
  var Action: TDataAction);
begin
	ProcessDeleteError(DataSet, E, Action);
end;

procedure TfrmMeal.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  begin
    FillTypesTree;
  	RefreshDataSets;
  end;
end;

procedure TfrmMeal.DBNavigatorEx1BeforeAction(Sender: TObject;
  Button: TNavigateBtn);
begin
	if Button = nbRefresh then
  begin
	  FillTypesTree;
  end;
end;

end.
