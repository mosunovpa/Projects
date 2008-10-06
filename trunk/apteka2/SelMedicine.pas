unit SelMedicine;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, Grids, DBGrids, Db, DBTables, DBCtrls, ADODB;

type
  TfrmSelMedicine = class(TForm)
    gridSelMedicine: TDBGrid;
    btnOk: TButton;
    btnCancel: TButton;
    Label1: TLabel;
    dsSelMedicine: TDataSource;
    edSearch: TEdit;
    timerSearch: TTimer;
    cmbSort: TComboBox;
    DBNavigator1: TDBNavigator;
    lblPatient: TLabel;
    tblAdoSelMedicine: TADOTable;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure gridSelMedicineDblClick(Sender: TObject);
    procedure timerSearchTimer(Sender: TObject);
    procedure btnCancelClick(Sender: TObject);
    procedure btnOkClick(Sender: TObject);
    procedure edSearchKeyPress(Sender: TObject; var Key: Char);
    procedure FormShow(Sender: TObject);
    procedure edSearchKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure cmbSortChange(Sender: TObject);
    procedure tblAdoSelMedicineAfterPost(DataSet: TDataSet);
    procedure tblAdoSelMedicineAfterDelete(DataSet: TDataSet);
    procedure tblAdoSelMedicineDeleteError(DataSet: TDataSet;
      E: EDatabaseError; var Action: TDataAction);
  private
    { Private declarations }

  public
    { Public declarations }
  end;

var
  frmSelMedicine: TfrmSelMedicine;

implementation

uses FMain, Storage;

{$R *.DFM}

procedure TfrmSelMedicine.FormCreate(Sender: TObject);
var
	FStorage: TStorage;
begin
  	tblAdoSelMedicine.Open;
    cmbSort.ItemIndex := 0;

	FStorage := TStorage.Create(Self.Name);
	try
		  FStorage.LoadDBGridState(gridSelMedicine);
    finally
    	FStorage.Free;
    end;
end;

procedure TfrmSelMedicine.FormDestroy(Sender: TObject);
var
	FStorage: TStorage;
begin
	FStorage := TStorage.Create(Self.Name);
	try
	  FStorage.StoreDBGridState(gridSelMedicine);
    finally
    	FStorage.Free;
    end;
end;

procedure TfrmSelMedicine.gridSelMedicineDblClick(Sender: TObject);
begin
	ModalResult := mrOk;
end;

procedure TfrmSelMedicine.edSearchKeyPress(Sender: TObject; var Key: Char);
begin
	timerSearch.Enabled := false;
	timerSearch.Enabled := true;
end;

procedure TfrmSelMedicine.timerSearchTimer(Sender: TObject);
var
	SearchField: string;
begin
	timerSearch.Enabled := false;
    case cmbSort.ItemIndex of
        0: SearchField := 'name';
        1: SearchField := 'sellname';
    end;
   	if tblAdoSelMedicine.Locate(SearchField, edSearch.Text, [loCaseInsensitive, loPartialKey]) then
        gridSelMedicine.SelectedIndex := cmbSort.ItemIndex;
end;

procedure TfrmSelMedicine.btnCancelClick(Sender: TObject);
begin
	if tblAdoSelMedicine.State <> dsBrowse then
	begin
    	ShowMessage('Закончите редактирование препарата');
        ModalResult := mrNone;
    end;

	timerSearch.Enabled := false;
end;

procedure TfrmSelMedicine.btnOkClick(Sender: TObject);
begin
	if tblAdoSelMedicine.State <> dsBrowse then
	begin
    	ShowMessage('Закончите редактирование препарата');
        ModalResult := mrNone;
    end;

	timerSearch.Enabled := false;
end;


procedure TfrmSelMedicine.FormShow(Sender: TObject);
begin
	edSearch.Text := '';
    edSearch.SetFocus;
end;

procedure TfrmSelMedicine.edSearchKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
	if (Key = VK_UP) or (Key = VK_DOWN) then
    begin
    	gridSelMedicine.Perform(WM_KEYDOWN, Key, 0);
    end;
end;

procedure TfrmSelMedicine.cmbSortChange(Sender: TObject);
begin
	case cmbSort.ItemIndex of
       0: tblAdoSelMedicine.IndexFieldNames := 'Name';
       1: tblAdoSelMedicine.IndexFieldNames := 'Sellname';
    end;
end;

procedure TfrmSelMedicine.tblAdoSelMedicineAfterPost(DataSet: TDataSet);
begin
    frmMain.Refresh(frmMain.tblAdoMedicine);
    frmMain.Refresh(frmMain.qryAdoPatientMedicine);
    frmMain.Refresh(frmMain.qryAdoSelectedMedicine);
end;

procedure TfrmSelMedicine.tblAdoSelMedicineAfterDelete(DataSet: TDataSet);
begin
    frmMain.Refresh(frmMain.tblAdoMedicine);
end;

procedure TfrmSelMedicine.tblAdoSelMedicineDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);
begin
	E.Message := 'Невозможно удалить препарат. '+
    'Возможно он назначен кому-либо из пациентов или выбран в отчете.' +
    #13#10 + E.Message;
end;

end.
