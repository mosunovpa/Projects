unit FDishes;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Grids, DBGrids, ExtCtrls, StdCtrls, DBCtrls, DB, ADODB,
  ComCtrls, Menus, ActnList, Storage, MDIChildForm, DBNavigatorEx, DBGridEx;

type
  TfrmDishes = class(TMDIChildForm)
    spltTypes: TSplitter;
    tblTypes: TADOTable;
    tblTypesId: TAutoIncField;
    tblTypesName: TWideStringField;
    tblDishes: TADOTable;
    tblDishesId: TAutoIncField;
    tblDishesName: TWideStringField;
    tblDishesTypeId: TIntegerField;
    tblDishesTypeName: TStringField;
    dsDishes: TDataSource;
    Panel2: TPanel;
    gridDishes: TDBGridEx;
    pnlComponents: TPanel;
    spltDishes: TSplitter;
    pnlDishes: TPanel;
    DBGrid2: TDBGridEx;
    pnlDishCategories: TPanel;
    Panel5: TPanel;
    tvTypes: TTreeView;
    tblDishComponents: TADOTable;
    dsDishComponents: TDataSource;
    tblDishComponentsDishId: TIntegerField;
    tblDishComponentsMealId: TIntegerField;
    tblDishComponentsQuantity: TBCDField;
    tblMeal: TADOTable;
    tblDishComponentsMealName: TStringField;
    DBNavigatorEx1: TDBNavigatorEx;
    DBNavigatorEx2: TDBNavigatorEx;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure tvTypesChange(Sender: TObject; Node: TTreeNode);
    procedure tblDishesBeforePost(DataSet: TDataSet);
    procedure tblDishesAfterInsert(DataSet: TDataSet);
    procedure dsDishesDataChange(Sender: TObject; Field: TField);
    procedure tblDishesDeleteError(DataSet: TDataSet; E: EDatabaseError;
      var Action: TDataAction);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure tblDishComponentsBeforePost(DataSet: TDataSet);
    procedure DBNavigatorEx2BeforeAction(Sender: TObject;
      Button: TNavigateBtn);
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
	rsDishesCaption = 'Блюда';

{ TfrmDishes }

procedure TfrmDishes.FillTypesTree;
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
	RootNode := tvTypes.Items.AddObject(nil, 'Блюда', Pointer(0));
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

procedure TfrmDishes.FormCreate(Sender: TObject);
begin
  FillTypesTree;
  OpenBigDataSet(tblMeal);
  OpenBigDataSet(tblDishes, 'Name');
  OpenBigDataSet(tblDishComponents);

	FCommonStorage.LoadComponentProp(pnlDishCategories, 'Width');
	FCommonStorage.LoadComponentProp(gridDishes, 'Height');
  FCommonStorage.LoadAllColumns;
end;

procedure TfrmDishes.FormDestroy(Sender: TObject);
begin
	FCommonStorage.SaveComponentProp(pnlDishCategories, 'Width');
	FCommonStorage.SaveComponentProp(gridDishes, 'Height');
  FCommonStorage.SaveAllColumns;
end;

procedure TfrmDishes.FormClose(Sender: TObject; var Action: TCloseAction);
begin
	Action := caFree;
end;

procedure TfrmDishes.tvTypesChange(Sender: TObject; Node: TTreeNode);
begin
	FTypeId := Integer(Node.Data);
	if FTypeId = 0 then
  begin
    tblDishes.Filtered := false;
  end
  else if FTypeId > 0 then
  begin
	  tblDishes.Filter := 'TypeId = ' + IntToStr(FTypeId);
  	tblDishes.Filtered := true;
  end;
  if tblDishes.Filtered = false then
  	pnlDishes.Caption := Format('%s:', [rsDishesCaption])
	else
	  pnlDishes.Caption := Format('%s (%s):', [rsDishesCaption, Node.Text]);
end;

procedure TfrmDishes.tblDishesBeforePost(DataSet: TDataSet);
begin
	if (tblDishes.Filtered = true)
  	and (tblDishesTypeId.Value <> FTypeId)
    then
  begin
  	if mrCancel = MessageDlg('Блюдо будет перемещено в категорию "' +
	    tblTypesName.AsString +'".' + #13#10 +
      'Сохранить изменения?',
    	mtConfirmation, [mbOk, mbCancel], 0) then
    begin
      Abort;
    end;
  end;
end;

procedure TfrmDishes.tblDishesAfterInsert(DataSet: TDataSet);
begin
	if tblDishes.Filtered = true then
  begin
    tblDishesTypeId.NewValue := FTypeId;
  end;
end;

procedure TfrmDishes.dsDishesDataChange(Sender: TObject; Field: TField);
begin
	if tblDishesName.AsString <> '' then
		pnlComponents.Caption := tblDishesName.AsString + ':'
  else
	  pnlComponents.Caption := 'Компоненты:';
end;

procedure TfrmDishes.tblDishesDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);
begin
	ProcessDeleteError(DataSet, E, Action);
end;

procedure TfrmDishes.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = VK_F5 then
  begin
  	FillTypesTree;
  	RefreshDataSets;
  end;
end;

procedure TfrmDishes.tblDishComponentsBeforePost(DataSet: TDataSet);
begin
	CheckNullField(TCustomADODataSet(DataSet));

end;

procedure TfrmDishes.DBNavigatorEx2BeforeAction(Sender: TObject;
  Button: TNavigateBtn);
begin
	if Button = nbRefresh then
		OpenBigDataSet(tblMeal);
end;

procedure TfrmDishes.DBNavigatorEx1BeforeAction(Sender: TObject;
  Button: TNavigateBtn);
begin
	if Button = nbRefresh then
  begin
	  FillTypesTree;
  end;
end;

end.
