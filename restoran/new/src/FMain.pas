unit FMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ActnList, Menus, StdActns, ComCtrls, ToolWin, ExtCtrls, Storage,
  MDIChildForm, AppEvnts, ImgList;

type
//	TMDIWindowTypes = (wtDishes, wtDishesTypes, wtMeal, wtMealTypes);

  TMainFormStorage = class(TFormStorage)
  private
    FMDIMaximized: Boolean;
  public
  	procedure Load;
    procedure Save;
  published
    property MDIMaximized: Boolean read FMDIMaximized write FMDIMaximized;
  end;

  TfrmMain = class(TForm)
    mnuMain: TMainMenu;
    N1: TMenuItem;
    ActionList: TActionList;
    N3: TMenuItem;
    acClose: TAction;
    acMealOperation: TAction;
    N2: TMenuItem;
    Meal1: TMenuItem;
    acDishesSelling: TAction;
    N4: TMenuItem;
    acDishTypes: TAction;
    acStorage: TAction;
    acMealTypes: TAction;
    N6: TMenuItem;
    N7: TMenuItem;
    N8: TMenuItem;
    N9: TMenuItem;
    N10: TMenuItem;
    WindowClose1: TWindowClose;
    Window1: TMenuItem;
    N11: TMenuItem;
    ArrangeAll1: TMenuItem;
    StatusBar: TStatusBar;
    acAbout: TAction;
    N12: TMenuItem;
    N13: TMenuItem;
    acDishes: TAction;
    acMeal: TAction;
    acDishes1: TMenuItem;
    acMeal1: TMenuItem;
    acMoveRests: TAction;
    acMoving: TAction;
    N5: TMenuItem;
    N14: TMenuItem;
    asSetDate: TAction;
    N15: TMenuItem;
    N16: TMenuItem;
    N18: TMenuItem;
    N19: TMenuItem;
    CoolBar: TCoolBar;
    tabWindows: TTabControl;
    ToolBar: TToolBar;
    ImageList1: TImageList;
    ToolButton1: TToolButton;
    ToolButton2: TToolButton;
    ToolButton3: TToolButton;
    ToolButton4: TToolButton;
    acCorrections: TAction;
    N17: TMenuItem;
    N20: TMenuItem;
    acDeleteData: TAction;
    N21: TMenuItem;
    acDeleteData1: TMenuItem;
    acMealSelling: TAction;
    N22: TMenuItem;
    N23: TMenuItem;
    AppEvents: TApplicationEvents;
    menuSelling: TPopupMenu;
    N24: TMenuItem;
    N25: TMenuItem;
    procedure acCloseExecute(Sender: TObject);
    procedure acAboutExecute(Sender: TObject);
    procedure acDishTypesExecute(Sender: TObject);
    procedure acMealTypesExecute(Sender: TObject);
    procedure acDishesExecute(Sender: TObject);
    procedure acMealExecute(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure tabWindowsChange(Sender: TObject);
    procedure acStorageExecute(Sender: TObject);
    procedure acMealOperationExecute(Sender: TObject);
    procedure acDishesSellingExecute(Sender: TObject);
    procedure acMovingExecute(Sender: TObject);
    procedure asSetDateExecute(Sender: TObject);
    procedure acMoveRestsExecute(Sender: TObject);
    procedure acMovingExExecute(Sender: TObject);
    procedure acCorrectionsExecute(Sender: TObject);
    procedure acDeleteDataExecute(Sender: TObject);
    procedure acMealSellingExecute(Sender: TObject);
  private
  	FStorage: TMainFormStorage;
		function CreateWindow(FormClass: TMDIFormClass): TMDIChildForm;
    procedure MDIActivate(Sender: TObject);
    procedure ChildCaptionChanged(Sender: TObject);
    procedure ChildDestroy(Sender: TObject);
    function GetTab(Obj: TObject): Integer;

  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses FAbout, FDishesTypes, FMealTypes, FDishes, FMeal, FDishSelling, FMealSelling,
  FOperation, FStorage, FMoving, FSetDate, FSetMonth, DMMain, FMovingPeriod,
  DMRestCalc, DBUtils, FMovingEx, FSetMovingDate, Math, FLogin, FCorrectRep,
  DateUtils, Constants;

{$R *.dfm}

const rsMDIMaximized: string = 'MDIMaximized';

{ TMainFormStorage }

procedure TMainFormStorage.Load;
begin
  LoadWindowState;
 	FMDIMaximized := ReadBool(rsMDIMaximized, false);
  with (FComponent as TfrmMain) do
  begin
  	LoadComponentProp(ToolBar, 'Width');
    LoadCollectionItemsProp(CoolBar, CoolBar.Bands, 'Break');
    LoadCollectionItemsProp(CoolBar, CoolBar.Bands, 'Width');
  end;
end;

procedure TMainFormStorage.Save;
begin
	if FComponent is TForm then
  begin
	  SaveWindowState;
		WriteBool(rsMDIMaximized, FMDIMaximized);
    with (FComponent as TfrmMain) do
    begin
	  	SaveComponentProp(ToolBar, 'Width');
      SaveCollectionItemsProp(CoolBar, CoolBar.Bands, 'Break');
      SaveCollectionItemsProp(CoolBar, CoolBar.Bands, 'Width');
    end;
  end;
end;

{ TfrmMain }

procedure TfrmMain.acCloseExecute(Sender: TObject);
begin
	Close;
end;

procedure TfrmMain.acAboutExecute(Sender: TObject);
var
	frmAbout: TfrmAbout;
begin
	frmAbout := TfrmAbout.Create(Application);
  frmAbout.Show;
end;

procedure TfrmMain.acDishTypesExecute(Sender: TObject);
begin
	CreateWindow(TfrmDishesTypes);
end;

procedure TfrmMain.acMealTypesExecute(Sender: TObject);
begin
	CreateWindow(TfrmMealTypes);
end;

procedure TfrmMain.acDishesExecute(Sender: TObject);
begin
	CreateWindow(TfrmDishes);
end;

procedure TfrmMain.acMealExecute(Sender: TObject);
begin
	CreateWindow(TfrmMeal);
end;

function TfrmMain.CreateWindow(FormClass: TMDIFormClass): TMDIChildForm;
var
	Form: TMDIChildForm;
	i: Integer;
  MDIChildMaxOrderNum: Integer;
begin
	Result := nil;
	if not FormClass.IsCanCreate then
  	Exit;

	Form := FormClass.Create(Application);
	// if first child window - restore window state
  if MDIChildCount = 1 then
    if FStorage.MDIMaximized = true then
	    Form.WindowState := wsMaximized;

  // set next order num and add it to form caption
	MDIChildMaxOrderNum := 0;
	for i := 0 to MDIChildCount - 1 do
   	if MDIChildren[i].ClassType = Form.ClassType then
    	if MDIChildren[i] is TMDIChildForm then
	    	if TMDIChildForm(MDIChildren[i]).OrderNum > MDIChildMaxOrderNum then
  	  		MDIChildMaxOrderNum := TMDIChildForm(MDIChildren[i]).OrderNum;

  Form.OrderNum := MDIChildMaxOrderNum + 1;
  Form.SetCaption(Form.Caption);

  // add to toolbar
  tabWindows.Tabs.AddObject(Form.Caption, Form);
  tabWindows.TabIndex := tabWindows.Tabs.Count - 1;

  // event when mdichild will be activated and destroyed
  Form.OnMDIActivate := MDIActivate;
  Form.OnChildDestroy := ChildDestroy;
  Form.OnCaptionChanged := ChildCaptionChanged;
  Form.Show;
  Result := Form;
end;

procedure TfrmMain.FormCreate(Sender: TObject);
begin
	FStorage := TMainFormStorage.Create(Self);
  FStorage.Load;
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
	FStorage.Save;
  FStorage.Free;
end;

procedure TfrmMain.ChildDestroy(Sender: TObject);
var
	i: Integer;
begin
	if Sender is TForm then
	  FStorage.MDIMaximized := TForm(Sender).WindowState = wsMaximized;

  i := GetTab(Sender);
  if i <> -1 then
  	tabWindows.Tabs.Delete(i);
end;

procedure TfrmMain.tabWindowsChange(Sender: TObject);
var
	Obj: TObject;
begin
	Obj := tabWindows.Tabs.Objects[tabWindows.TabIndex];
  if Obj is TForm then
  	TForm(Obj).Show;
end;

procedure TfrmMain.MDIActivate(Sender: TObject);
var
	i: Integer;
begin
	i := GetTab(Sender);
	if i <> -1 then
    tabWindows.TabIndex := i;
end;

function TfrmMain.GetTab(Obj: TObject): Integer;
var
	i: Integer;
begin
  Result := -1;
	for i := 0 to tabWindows.Tabs.Count - 1 do
		if tabWindows.Tabs.Objects[i] = Obj then
    begin
      Result := i;
      Exit;
    end;
end;

procedure TfrmMain.acStorageExecute(Sender: TObject);
begin
	CreateWindow(TfrmStorage);
end;

procedure TfrmMain.acMealOperationExecute(Sender: TObject);
begin
	CreateWindow(TfrmOperations);
end;

procedure TfrmMain.acDishesSellingExecute(Sender: TObject);
begin
	CreateWindow(TfrmDishSelling);
end;

procedure TfrmMain.acMealSellingExecute(Sender: TObject);
begin
	CreateWindow(TfrmMealSelling);
end;

procedure TfrmMain.acMovingExecute(Sender: TObject);
var
	Form: TMDIChildForm;
begin
	with TfrmSetMovingDate.Create(Application) do
    try
      if ShowModal = mrOk then
        if Sheet.ActivePage = PageDate then
        begin
					Form := CreateWindow(TfrmMoving);
          if Form <> nil then
          begin
	          TfrmMoving(Form).calendarDate.Date := Floor(calendarDate.Date);
          	TfrmMoving(Form).ShowMoving;
          end
        end
        else
        begin
					Form := CreateWindow(TfrmMovingEx);
          if Form <> nil then
          begin
	          TfrmMovingEx(Form).calendarDateFrom.Date := Floor(calendarFrom.Date);
	          TfrmMovingEx(Form).calendarDateTo.Date := Floor(calendarTo.Date);
          	TfrmMovingEx(Form).ShowMoving;
          end
        end;
    finally
      Free;
    end;
end;

procedure TfrmMain.asSetDateExecute(Sender: TObject);
var
	frmSetDate: TfrmSetDate;
begin
	frmSetDate := TfrmSetDate.Create(Application);
  try
	  frmSetDate.calendar.Date := dmodMain.CurDate;
  	if (frmSetDate.ShowModal = mrOk) then
	    dmodMain.CurDate := frmSetDate.calendar.Date;

  finally
    frmSetDate.Free;
  end;
end;

procedure TfrmMain.acMoveRestsExecute(Sender: TObject);
var
  Save_Cursor: TCursor;
begin
	with TfrmMovingPeriod.Create(Application) do
	  try
  		if (ShowModal = mrOk) and (BeginDate <> 0) and (EndDate <> 0) then
      begin
        Save_Cursor := Screen.Cursor;
        try
	        Screen.Cursor := crHourGlass;
        	dmodRestCalc.CalculateRest(BeginDate, EndDate);
        finally
        	Screen.Cursor := Save_Cursor;
        end;
        MessageDlg('Остатки успешно перенесены.', mtInformation, [mbOk], 0)
      end;
	  finally
  	  Free;
	  end;
end;

procedure TfrmMain.ChildCaptionChanged(Sender: TObject);
var
	i: Integer;
begin
	i := GetTab(Sender);
	if i <> -1 then
    tabWindows.Tabs[i] := TForm(Sender).Caption;
end;

procedure TfrmMain.acMovingExExecute(Sender: TObject);
begin
	CreateWindow(TfrmMovingEx);
end;

procedure TfrmMain.acCorrectionsExecute(Sender: TObject);
var
	frmSetDate: TfrmSetDate;
begin
	frmSetDate := TfrmSetDate.Create(Application);
  try
  	if (Self.ActiveMDIChild is TfrmMoving) then
			frmSetDate.calendar.Date :=
      	(Self.ActiveMDIChild as TfrmMoving).CurrentDate
    else if (Self.ActiveMDIChild is TfrmMovingEx) then
			frmSetDate.calendar.Date :=
      	(Self.ActiveMDIChild as TfrmMovingEx).CurrentDate
    else
		  frmSetDate.calendar.Date := dmodMain.CurDate;

    frmSetDate.Caption := 'Дата корректировок';
    frmSetDate.lblTitle.Caption := 'Укажите дату корректировок:';
  	if (frmSetDate.ShowModal = mrOk) then
      ShowCorrectionsReport(Floor(frmSetDate.calendar.Date));
  finally
    frmSetDate.Free;
  end;
end;

procedure TfrmMain.acDeleteDataExecute(Sender: TObject);
var
  frmSetMonth: TfrmSetMonth;
  BeginDate, EndDate: TDateTime;
  StorageExist: Boolean;
  Save_Cursor: TCursor;
begin
	StorageExist := dmodRestCalc.GetMinMaxStorageDate(BeginDate, EndDate);
  if StorageExist = True then
	begin
    if EndDate > dmodMain.CurDate then
      EndDate := StartOfTheMonth(dmodMain.CurDate);

    if BeginDate > EndDate then
    	StorageExist := False;
	end;

  if StorageExist = False then
	begin
  	MessageDlg('Отсутствуют данные для удаления', mtInformation, [mbOk], 0);
    Exit;
	end;

  frmSetMonth := TfrmSetMonth.Create(Application);
  try
    frmSetMonth.comboMonth.ShowDay := True;
    frmSetMonth.comboMonth.Fill(BeginDate, EndDate);
    frmSetMonth.comboMonth.Date := EndDate;
    if (frmSetMonth.ShowModal = mrOk) then
      if mrOk = MessageDlg('Все данные до "' +
        DateToStr(Floor(frmSetMonth.comboMonth.Date)) +'" будут удалены.' + #13#10 +
        'Удалить данные?',
        mtConfirmation, [mbOk, mbCancel], 0) then
      begin
        Save_Cursor := Screen.Cursor;
        try
          Screen.Cursor := crHourGlass;
          dmodRestCalc.DeleteData(Floor(frmSetMonth.comboMonth.Date));
        finally
          Screen.Cursor := Save_Cursor;
        end;
        MessageDlg('Данные успешно удалены.', mtInformation, [mbOk], 0)
      end;
  finally
    frmSetMonth.Free;
  end;
end;


end.

