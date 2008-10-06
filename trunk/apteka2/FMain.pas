unit FMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  DBTables, Grids, DBGrids, Db, Menus, ComCtrls, StdCtrls, Buttons,
  ExtCtrls, DBCtrls, FileCtrl, ADODB, Variants;

const
	WM_SETUP = WM_USER + 400;

type
  TSearchObject = (soPatient, soMedicine);

  TfrmMain = class(TForm)
    dsPatient: TDataSource;
    mnuMain: TMainMenu;
    File1: TMenuItem;
    mnuExit: TMenuItem;
    Pages: TPageControl;
    tsPatient: TTabSheet;
    tsMedicine: TTabSheet;
    splitPatient: TSplitter;
    dsMedicine: TDataSource;
    dsPatientMedicine: TDataSource;
    dsMedicinePatient: TDataSource;
    panelMedicineBottom: TPanel;
    Panel6: TPanel;
    Bevel2: TBevel;
    btnDosa2: TSpeedButton;
    btnGotoPatient: TSpeedButton;
    panelMedicinePatient: TPanel;
    gridMedicinePatient: TDBGrid;
    panelPatientBottom: TPanel;
    Panel5: TPanel;
    btnAdd: TSpeedButton;
    Bevel1: TBevel;
    btnDosa: TSpeedButton;
    btnDel: TSpeedButton;
    btnGotoMedicine: TSpeedButton;
    panelPatientMedicine: TPanel;
    gridPatientMedicine: TDBGrid;
    panelPatientTop: TPanel;
    gridPatient: TDBGrid;
    Panel2: TPanel;
    Panel7: TPanel;
    DBNavigator1: TDBNavigator;
    edPatientSearch: TEdit;
    panelMedicine: TPanel;
    panelMedicineLeft: TPanel;
    gridMedicine: TDBGrid;
    Panel3: TPanel;
    Panel8: TPanel;
    DBNavigator2: TDBNavigator;
    edMedicineSearch: TEdit;
    Splitter1: TSplitter;
    Splitter2: TSplitter;
    panelMedicineRight: TPanel;
    Panel1: TPanel;
    gridSelectedMedicine: TDBGrid;
    timerSearch: TTimer;
    Bevel3: TBevel;
    btnSave: TSpeedButton;
    btnSelect: TSpeedButton;
    btnDeselect: TSpeedButton;
    dsSelectedMedicine: TDataSource;
    savedlgReport: TSaveDialog;
    cmbSort: TComboBox;
    btnCount: TSpeedButton;
    AdoConnection: TADOConnection;
    tblAdoPatient: TADOTable;
    tblAdoMedicine: TADOTable;
    tblAdoPatientMedicine: TADOTable;
    qryAdoPatientMedicine: TADOQuery;
    qryAdoMedicinePatient: TADOQuery;
    qryAdoSelectedMedicine: TADOQuery;
    tblAdoSelectedMedicine: TADOTable;
    qryAdoReport: TADOQuery;
    qryAdoTemp: TADOQuery;
    procedure mnuExitClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure btnAddClick(Sender: TObject);
    procedure btnDelClick(Sender: TObject);
    procedure btnDosaClick(Sender: TObject);
    procedure tblMedicineAfterPost(DataSet: TDataSet);
    procedure tblMedicineAfterDelete(DataSet: TDataSet);
    procedure tblPatientAfterPost(DataSet: TDataSet);
    procedure gridPatientMedicineDblClick(Sender: TObject);
    procedure btnDosa2Click(Sender: TObject);
    procedure gridMedicinePatientDblClick(Sender: TObject);
    procedure tblPatientAfterScroll(DataSet: TDataSet);
    procedure btnGotoMedicineClick(Sender: TObject);
    procedure btnGotoPatientClick(Sender: TObject);
    procedure tblMedicineAfterScroll(DataSet: TDataSet);
    procedure timerSearchTimer(Sender: TObject);
    procedure edPatientSearchKeyPress(Sender: TObject; var Key: Char);
    procedure edMedicineSearchKeyPress(Sender: TObject; var Key: Char);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure edPatientSearchKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure edMedicineSearchKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure btnSelectClick(Sender: TObject);
    procedure btnDeselectClick(Sender: TObject);
    procedure qrySelectedMedicineAfterScroll(DataSet: TDataSet);
    procedure btnSaveClick(Sender: TObject);
    procedure cmbSortChange(Sender: TObject);
    procedure gridSelectedMedicineDblClick(Sender: TObject);
    procedure btnCountClick(Sender: TObject);
    procedure gridSelectedMedicineKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure tblAdoMedicineDeleteError(DataSet: TDataSet;
      E: EDatabaseError; var Action: TDataAction);
    procedure tblAdoPatientDeleteError(DataSet: TDataSet;
      E: EDatabaseError; var Action: TDataAction);
  private
    { Private declarations }
    FSearchObject: TSearchObject;
    procedure LoadState;
    procedure SaveState;
	procedure EditDosa(query: TADOQuery; new: Boolean; patient: string; medicine: string);
    procedure OpenTables;
    procedure CloseTables;
    procedure StoreToArchive;
    procedure SetCount;
	procedure UpdateMedicinePatientCaption(id: integer);
    function GetPackages(MedicineId: integer): integer;
  public
    procedure Refresh(query: TDataSet);
  protected
	 procedure MWMSetup(var Message: TMessage); message WM_SETUP;
  end;

var
  frmMain: TfrmMain;

implementation

uses Storage, SelMedicine, FQuantity, FCount, ComObj;

{$R *.DFM}

procedure TfrmMain.mnuExitClick(Sender: TObject);
begin
	Close;
end;


procedure TfrmMain.FormCreate(Sender: TObject);
begin
    PostMessage(Handle, WM_SETUP, 0, 0);

    savedlgReport.InitialDir := ExtractFilePath(Application.ExeName) + 'reports';
    if not DirectoryExists(savedlgReport.InitialDir) then
    begin
		CreateDir(savedlgReport.InitialDir);
    end;


	AdoConnection.ConnectionString :=
    	'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=' +
        ExtractFilePath(Application.ExeName) +
        'data\database.mdb;Persist Security Info=False';

    OpenTables;
    Pages.ActivePage := tsPatient;
    ActiveControl := gridPatient;
end;

procedure TfrmMain.MWMSetup(var Message: TMessage);
begin
	LoadState;
	inherited;
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
	SaveState;
end;

procedure TfrmMain.LoadState;
var
	r: TRect;
    height: integer;
	FStorage: TStorage;
    i: integer;
begin
    FStorage := TStorage.Create(Self.Name);
	try
    	if FStorage.ReadBool('frmMain_WindowMaximized', false) then
        	WindowState := wsMaximized;
		for i := 0 to ComponentCount - 1 do
			if Components[i] is TDBGrid then
			  FStorage.LoadDBGridState(TDBGrid(Components[i]));
    finally
    	FStorage.Free;
    end;

	r := GetClientRect;
    height := (r.Bottom div 5) * 3;
	panelPatientTop.Height := height;
	panelMedicine.Height := height;
    panelMedicineLeft.Width := r.Right div 2;

    cmbSort.ItemIndex := 0;
end;

procedure TfrmMain.OpenTables;
begin
	AdoConnection.Open;
	tblAdoPatient.Open;
	tblAdoMedicine.Open;
    qryAdoPatientMedicine.Open;
    qryAdoMedicinePatient.Open;
    tblAdoPatientMedicine.Open;
    qryAdoSelectedMedicine.Open;
    tblAdoSelectedMedicine.Open;
end;

procedure TfrmMain.CloseTables;
begin
	AdoConnection.Close;
end;

procedure TfrmMain.StoreToArchive;
var
	ArchDir: string;
    CmdLine: string;
    CurDir: string;
begin
	if MessageDlg('Архивировать данные?', mtConfirmation, [mbYes, mbNo], 0) = mrNo then
		Exit;
        
	CurDir := ExtractFilePath(Application.ExeName);
	ArchDir := CurDir + 'archive';
    if not DirectoryExists(ArchDir) then
    begin
		CreateDir(ArchDir);
    end;
    CmdLine := CurDir + 'rar.exe a ' + ArchDir + FormatDateTime(
    	'"\data_"yyyy"_"mm"_"dd"_"hh"h_"nn"m.rar ', Now) + CurDir + 'data';

    WinExec(PChar(CmdLine), SW_SHOW);
end;

procedure TfrmMain.SaveState;
var
	FStorage: TStorage;
    i: integer;
begin
	FStorage := TStorage.Create(Name);
	try
    	FStorage.WriteBool('frmMain_WindowMaximized',
        	WindowState = wsMaximized);
		for i := 0 to ComponentCount - 1 do
			if Components[i] is TDBGrid then
			  FStorage.StoreDBGridState(TDBGrid(Components[i]));
    finally
    	FStorage.Free;
    end;
end;

procedure TfrmMain.btnAddClick(Sender: TObject);
var
	id_patient: integer;
	id_medicine: integer;
begin
	if tblAdoPatient.State <> dsBrowse then
    begin
		tblAdoPatient.Post;
    end;
	frmSelMedicine.lblPatient.Caption := 'Пациент: ' +
    	tblAdoPatient.FieldByName('lastname').AsString + ' ' +
    	tblAdoPatient.FieldByName('firstname').AsString + ' ' +
    	tblAdoPatient.FieldByName('secondname').AsString;
	if frmSelMedicine.ShowModal = mrOk then
    begin
	    id_patient := tblAdoPatient.FieldByName('id').Value;
        id_medicine := frmSelMedicine.tblAdoSelMedicine.FieldByName('id').Value;
    	if qryAdoPatientMedicine.Locate('id_patient;id_medicine',
	        VarArrayOf([id_patient, id_medicine]), []) then
        begin
            MessageDlg('Препарат уже назначен.', mtInformation, [mbOk], 0);
			Exit;
        end;

        tblAdoPatientMedicine.Insert;
        try
            tblAdoPatientMedicine.FieldByName('id_patient').Value := id_patient;
            tblAdoPatientMedicine.FieldByName('id_medicine').Value := id_medicine;
            tblAdoPatientMedicine.FieldByName('quantity').Value := 1;
            tblAdoPatientMedicine.Post;

            qryAdoPatientMedicine.Close;
            qryAdoPatientMedicine.Open;
            if qryAdoPatientMedicine.Locate('id_patient;id_medicine',
                VarArrayOf([id_patient, id_medicine]), []) then
            begin
				EditDosa(qryAdoPatientMedicine, true,
                	tblAdoPatient.FieldByName('lastname').AsString,
                    qryAdoPatientMedicine.FieldByName('sellname').AsString + ' (' +
                    qryAdoPatientMedicine.FieldByName('name').AsString + ')');
            end;

        except
	       	tblAdoPatientMedicine.Cancel;
            raise;
        end;
    end;
end;

procedure TfrmMain.EditDosa(query: TADOQuery; new: Boolean; patient: string; medicine: string);
begin
    if tblAdoPatientMedicine.Locate('id_patient;id_medicine',
        VarArrayOf([query.FieldByName('id_patient').Value,
            query.FieldByName('id_medicine').Value]), []) then
    begin
	    frmQuantity.lblPatient.Caption := patient;
	    frmQuantity.lblMedicine.Caption := medicine;
	    frmQuantity.edDosa.Text := query.FieldByName('dosa').AsString;
		frmQuantity.updwQuantity.Position := query.FieldByName('quantity').AsInteger;
        if new then
        	frmQuantity.edDescription.Text := 'назн'
        else
		    frmQuantity.edDescription.Text := query.FieldByName('description').AsString;

	    if frmQuantity.ShowModal = mrOk then
        begin
            tblAdoPatientMedicine.Edit;
            try
                tblAdoPatientMedicine.FieldByName('dosa').AsString :=
                    frmQuantity.edDosa.Text;
                tblAdoPatientMedicine.FieldByName('quantity').AsInteger :=
                    frmQuantity.updwQuantity.Position;
                tblAdoPatientMedicine.FieldByName('description').AsString :=
                    frmQuantity.edDescription.Text;
                tblAdoPatientMedicine.Post;
            except
                tblAdoPatientMedicine.Cancel;
                raise;
            end;
            Refresh(qryAdoPatientMedicine);
			Refresh(qryAdoMedicinePatient);
		    UpdateMedicinePatientCaption(tblAdoPatientMedicine.FieldByName('id_medicine').AsInteger);
        end;
    end;
end;

procedure TfrmMain.btnDelClick(Sender: TObject);
begin
    if tblAdoPatientMedicine.Locate('id_patient;id_medicine',
        VarArrayOf([qryAdoPatientMedicine.FieldByName('id_patient').Value,
            qryAdoPatientMedicine.FieldByName('id_medicine').Value]), []) then
    begin
		if MessageDlg('Отменить назначение?', mtConfirmation, mbOkCancel, 0) = mrOk then
        begin
            tblAdoPatientMedicine.Delete;
            qryAdoPatientMedicine.Close;
            qryAdoPatientMedicine.Open;
            qryAdoMedicinePatient.Close;
            qryAdoMedicinePatient.Open;
        end;
    end;
end;

procedure TfrmMain.tblMedicineAfterPost(DataSet: TDataSet);
begin
    Refresh(frmSelMedicine.tblAdoSelMedicine);
	Refresh(qryAdoPatientMedicine);
	Refresh(qryAdoSelectedMedicine);
end;

procedure TfrmMain.tblMedicineAfterDelete(DataSet: TDataSet);
begin
    Refresh(frmSelMedicine.tblAdoSelMedicine);
end;

procedure TfrmMain.Refresh(query: TDataSet);
var
	bookmark: TBookmark;
begin
    try
        bookmark := query.GetBookmark;
        try
            query.Close;
            query.Open;
            query.GotoBookmark(bookmark);
        finally
            query.FreeBookmark(bookmark);
        end;
    except
    end;
end;

procedure TfrmMain.tblPatientAfterPost(DataSet: TDataSet);
begin
	Refresh(qryAdoMedicinePatient);
end;

procedure TfrmMain.gridPatientMedicineDblClick(Sender: TObject);
begin
    btnDosaClick(Sender);
end;

procedure TfrmMain.btnDosaClick(Sender: TObject);
begin
	EditDosa(qryAdoPatientMedicine, false,
            tblAdoPatient.FieldByName('lastname').AsString,
            qryAdoPatientMedicine.FieldByName('sellname').AsString + ' (' +
            qryAdoPatientMedicine.FieldByName('name').AsString + ')');
end;

procedure TfrmMain.btnDosa2Click(Sender: TObject);
begin
	EditDosa(qryAdoMedicinePatient, false,
            qryAdoMedicinePatient.FieldByName('lastname').AsString,
            tblAdoMedicine.FieldByName('sellname').AsString + ' (' +
            tblAdoMedicine.FieldByName('name').AsString + ')');
end;

procedure TfrmMain.gridMedicinePatientDblClick(Sender: TObject);
begin
    btnDosa2Click(Sender);
end;

procedure TfrmMain.tblPatientAfterScroll(DataSet: TDataSet);
begin
	panelPatientMedicine.Caption := 'Назначенные препараты (' +
        tblAdoPatient.FieldByName('Lastname').AsString + ' ' +
        tblAdoPatient.FieldByName('Firstname').AsString + ' ' +
        tblAdoPatient.FieldByName('Secondname').AsString + '):'

end;

procedure TfrmMain.btnGotoMedicineClick(Sender: TObject);
begin
	if not qryAdoPatientMedicine.IsEmpty then
    begin
        if tblAdoMedicine.Locate('id', qryAdoPatientMedicine.FieldByName('id_medicine').Value, []) then
        begin
			qryAdoMedicinePatient.Locate('id_patient', qryAdoPatientMedicine.FieldByName('id_patient').Value, []);
            Pages.ActivePage := tsMedicine;
        end;
    end;
end;

procedure TfrmMain.btnGotoPatientClick(Sender: TObject);
begin
	if not qryAdoMedicinePatient.IsEmpty then
    begin
        if tblAdoPatient.Locate('id', qryAdoMedicinePatient.FieldByName('id_patient').Value, []) then
        begin
			qryAdoPatientMedicine.Locate('id_medicine', qryAdoMedicinePatient.FieldByName('id_medicine').Value, []);
            Pages.ActivePage := tsPatient;
        end;
    end;
end;

procedure TfrmMain.tblMedicineAfterScroll(DataSet: TDataSet);
var
	id: integer;
begin
  	id := tblAdoMedicine.FieldByName('id').AsInteger;

    UpdateMedicinePatientCaption(id);

    if qryAdoSelectedMedicine.Active then
    begin
        if id <> qryAdoSelectedMedicine.FieldByName('id').AsInteger then
		    qryAdoSelectedMedicine.Locate('id', id, []);

    end;
end;

procedure TfrmMain.UpdateMedicinePatientCaption(id: integer);
var
    AllPackagesStr: string;
    AllPackages: integer;
begin
    AllPackages := GetPackages(id);
    if AllPackages > 0 then
	   	AllPackagesStr := ', всего ' + IntToStr(AllPackages) + ' упаковок';

    panelMedicinePatient.Caption := 'Кому назначен (' +
    	tblAdoMedicine.FieldByName('name').AsString + AllPackagesStr + '):';
end;

procedure TfrmMain.timerSearchTimer(Sender: TObject);
var
	SearchField: string;
begin
    timerSearch.Enabled := false;
    case FSearchObject of
      soPatient:
      	if tblAdoPatient.Locate('lastname', edPatientSearch.Text, [loCaseInsensitive, loPartialKey]) then
			gridPatient.SelectedIndex := 0;
      soMedicine:
      	begin
            case cmbSort.ItemIndex of
                0: SearchField := 'name';
                1: SearchField := 'sellname';
            end;
			if tblAdoMedicine.Locate(SearchField, edMedicineSearch.Text,
    	    	[loCaseInsensitive, loPartialKey]) then
            begin
                gridMedicine.SelectedIndex := cmbSort.ItemIndex;
            end;
        end;
    end;
end;

procedure TfrmMain.edPatientSearchKeyPress(Sender: TObject; var Key: Char);
begin
    timerSearch.Enabled := false;
    FSearchObject := soPatient;
    timerSearch.Enabled := true;
end;

procedure TfrmMain.edMedicineSearchKeyPress(Sender: TObject;
  var Key: Char);
begin
    timerSearch.Enabled := false;
    FSearchObject := soMedicine;
    timerSearch.Enabled := true;
end;

procedure TfrmMain.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
	if (Pages.ActivePage = tsPatient) and (Key = VK_F7) then
	    btnAddClick(Sender);
end;

procedure TfrmMain.edPatientSearchKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
	if (Key = VK_UP) or (Key = VK_DOWN) then
	begin
	    gridPatient.SetFocus;
    	gridPatient.Perform(WM_KEYDOWN, Key, 0);
    end;
end;

procedure TfrmMain.edMedicineSearchKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
	if (Key = VK_UP) or (Key = VK_DOWN) then
    begin
	    gridMedicine.SetFocus;
    	gridMedicine.Perform(WM_KEYDOWN, Key, 0);
    end;
end;

procedure TfrmMain.btnSelectClick(Sender: TObject);
var
	id: integer;
begin
	id := tblAdoMedicine.FieldByName('id').AsInteger;
    if id > 0 then
    begin
    	// check if already exist
		if qryAdoSelectedMedicine.Locate('id', id, []) then
		begin
            MessageDlg('Препарат уже выбран.', mtInformation, [mbOk], 0);
			Exit;
		end;

        // insert
		tblAdoSelectedMedicine.Insert;
        try
			tblAdoSelectedMedicine.FieldByName('id').AsInteger := id;
            tblAdoSelectedMedicine.Post;
        except
        	tblAdoSelectedMedicine.Cancel;
            raise;
        end;
        qryAdoSelectedMedicine.Close;
        qryAdoSelectedMedicine.Open;
        qryAdoSelectedMedicine.Locate('id', id, []);
    end;
end;

procedure TfrmMain.btnDeselectClick(Sender: TObject);
begin
	if tblAdoSelectedMedicine.Locate('id',
    	qryAdoSelectedMedicine.FieldByName('id').AsInteger, []) then
    begin
        tblAdoSelectedMedicine.Delete;
        qryAdoSelectedMedicine.Close;
        qryAdoSelectedMedicine.Open;
    end;
end;

procedure TfrmMain.qrySelectedMedicineAfterScroll(DataSet: TDataSet);
var
	id: integer;
begin
	id := qryAdoSelectedMedicine.FieldByName('id').AsInteger;
    if id <> tblAdoMedicine.FieldByName('id').AsInteger then
	    tblAdoMedicine.Locate('id', id, []);
end;

procedure TfrmMain.btnSaveClick(Sender: TObject);
var
    F: TextFile;
    number: integer;
	current_med_id: integer;
    current_count: integer;
    current_sum: integer;
    show_row: boolean;
begin
	current_med_id := 0;
    current_count := 0;
    current_sum := 0;
	if tblAdoSelectedMedicine.IsEmpty then
    begin
        MessageDlg('Выберите препараты для отчета', mtInformation, [mbOk], 0);
        Exit;
    end;
    if savedlgReport.Execute then
    begin
        Screen.Cursor := crHourglass;
        try
            AssignFile(F, savedlgReport.FileName);
            Rewrite(F);
            try
                Writeln(F, 'ПЕРСОНИФИЦИРОВАННЫЕ СПИСКИ ЛЬГОТНИКОВ, ДЛЯ КОТОРЫХ ЗАЯВЛЯЮТСЯ ЛЕКАРСТВЕННЫЕ СРЕДСТВА');
                Writeln(F, 'НАИМЕНОВАНИЕ ЛПУ МУЗ "БЦГБ"');
                Writeln(F, 'КОД ЛПУ 647');

                Writeln(F,
                    '№ п/п;Фамилия;Имя;Отчество;Дата рождения;Адрес;' +
                    'Код категории;Дата установления инвалидности;Диагноз;' +
                    'МНН;Торговое наименование, дозировка, форма выпуска;'+
                    'Разовая доза назначения препарата в сутки;'+
                    'Количество упаковок на месяц;Обоснование обозначения препарата');

                with qryAdoReport do begin
                    Open;
                    try
                        number := 0;
                        while not Eof do
                        begin
                            show_row := true;
                            if FieldByName('Count').AsInteger <> 0 then
                            begin
                                if FieldByName('id').AsInteger <> current_med_id then
                                begin
                                    current_med_id := FieldByName('id').AsInteger;
                                    current_count := FieldByName('Count').AsInteger;
                                    current_sum := 0;
                                end;
                                current_sum := current_sum + FieldByName('Quantity').AsInteger;
                                if current_sum > current_count then
                                    show_row := false;
                            end;

                            if show_row = true then
                            begin
                                number := number + 1;
                                Writeln(F,
                                    IntToStr(number) + ';' +
                                    FieldByName('Lastname').AsString + ';' +
                                    FieldByName('Firstname').AsString + ';' +
                                    FieldByName('Secondname').AsString + ';' +
                                    FieldByName('Birthday').AsString + ';' +
                                    FieldByName('Address').AsString + ';' +
                                    FieldByName('Code').AsString + ';' +
                                    FieldByName('Invalid').AsString + ';' +
                                    FieldByName('Diagnos').AsString + ';' +
                                    FieldByName('Name').AsString + ';' +
                                    FieldByName('Sellname').AsString + ';' +
                                    FieldByName('Dosa').AsString + ';' +
                                    FieldByName('Quantity').AsString + ';' +
                                    FieldByName('Description').AsString);
                            end;
                            Next;
                        end;
                    finally
                        Close;
                    end;
                end;
            finally
                CloseFile(F);
            end;
        finally
            Screen.Cursor := crDefault;
        end;
    end;
end;

procedure TfrmMain.cmbSortChange(Sender: TObject);
begin
	case cmbSort.ItemIndex of
       0: tblAdoMedicine.IndexFieldNames := 'Name';
       1: tblAdoMedicine.IndexFieldNames := 'Sellname';
    end;
end;

procedure TfrmMain.gridSelectedMedicineDblClick(Sender: TObject);
begin
     SetCount;
end;

procedure TfrmMain.btnCountClick(Sender: TObject);
begin
     SetCount;
end;

procedure TfrmMain.SetCount;
var
	id: integer;
begin
	frmCount.Count := qryAdoSelectedMedicine.FieldByName('Count').AsInteger;
	if frmCount.ShowModal = mrOk then
    begin
    	id := qryAdoSelectedMedicine.FieldByName('id').AsInteger;
    	if (tblAdoSelectedMedicine.Locate('id', id, [])) then
        begin
             tblAdoSelectedMedicine.Edit;
             try
             	if frmCount.Count = 0 then
    	            tblAdoSelectedMedicine.FieldByName('Count').Clear
                else
	                tblAdoSelectedMedicine.FieldByName('Count').AsInteger := frmCount.Count;
                tblAdoSelectedMedicine.Post;
             except
                tblAdoSelectedMedicine.Cancel;
                raise;
            end;
            qryAdoSelectedMedicine.Close;
            qryAdoSelectedMedicine.Open;
            qryAdoSelectedMedicine.Locate('id', id, []);
        end;
     end;
end;

procedure TfrmMain.gridSelectedMedicineKeyDown(Sender: TObject;
  var Key: Word; Shift: TShiftState);
begin
	if Key = VK_RETURN then
    begin
    	SetCount;
    end;
end;

procedure TfrmMain.FormClose(Sender: TObject; var Action: TCloseAction);
begin
    CloseTables;
    StoreToArchive;
end;

function TfrmMain.GetPackages(MedicineId: integer): integer;
begin
    qryAdoTemp.Sql.Clear;
    qryAdoTemp.Sql.Text := 'select sum(Quantity) as cnt from patient_medicine where id_medicine = :id';
    qryAdoTemp.Parameters.ParseSql(qryAdoTemp.Sql.Text, true);
    qryAdoTemp.Parameters.ParamByName('id').Value := MedicineId;
    qryAdoTemp.Open;
    Result := qryAdoTemp.FieldByName('cnt').AsInteger;
    qryAdoTemp.Close;
end;

procedure TfrmMain.tblAdoMedicineDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);
begin
	E.Message := 'Невозможно удалить препарат. '+
    'Возможно он назначен кому-либо из пациентов или выбран в отчете.' +
    #13#10 + E.Message;
end;

procedure TfrmMain.tblAdoPatientDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);
begin
	E.Message := 'Невозможно удалить пациента. '+
    'Отмените все назначенные пациенту препараты перед удалением.' +
    #13#10 + E.Message;
end;

end.
