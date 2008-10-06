unit MDIChildForm;

interface

uses
	Forms, Classes, Messages, Storage;

type
	TMDIChildStorage = class(TFormStorage)
	public
  	procedure LoadAllColumns;
    procedure SaveAllColumns;
  end;

	TMDIChildForm = class(TForm)
 	private
    FOnMDIActivate: TNotifyEvent;
    FOnMDIDeactivate: TNotifyEvent;
	  FOnChildDestroy: TNotifyEvent;
	  FOnCaptionChanged: TNotifyEvent;
    FOrderNum: Integer;
    procedure WMMDIActivate(var Message: TWMMDIActivate); message WM_MDIACTIVATE;
    procedure WMCreate(var Message: TWMCreate); message WM_CREATE;
    procedure WMDestroy(var Message: TWMDestroy); message WM_DESTROY;
    procedure WMChar(var Message: TWMChar); message WM_CHAR;
	protected
    FCommonStorage: TMDIChildStorage;
    procedure RefreshDataSets; dynamic;
  public
	  class function IsCanCreate: boolean; dynamic;
	  procedure SetCaption(NewCaption: string);
  published
  	property OnMDIActivate: TNotifyEvent read FOnMDIActivate write FOnMDIActivate;
  	property OnMDIDeactivate: TNotifyEvent read FOnMDIDeactivate write FOnMDIDeactivate;
  	property OnChildDestroy: TNotifyEvent read FOnChildDestroy write FOnChildDestroy;
  	property OnCaptionChanged: TNotifyEvent read FOnCaptionChanged write FOnCaptionChanged;
    property OrderNum: Integer read FOrderNum write FOrderNum;
  end;

  TMDIFormClass = class of TMDIChildForm;

implementation

uses
	DBGrids, DB, SysUtils, DBUtils, ADODB, Windows;

{ TMDIChildStorage }

procedure TMDIChildStorage.LoadAllColumns;
var
	i: Integer;
begin
	for i := 0 to FComponent.ComponentCount - 1 do
		if FComponent.Components[i] is TDBGrid then
		  LoadCollectionItemsProp(TDBGrid(FComponent.Components[i]),
  	  	TDBGrid(FComponent.Components[i]).Columns, 'Width');
end;

procedure TMDIChildStorage.SaveAllColumns;
var
	i: Integer;
begin
	for i := 0 to FComponent.ComponentCount - 1 do
		if FComponent.Components[i] is TDBGrid then
		  SaveCollectionItemsProp(TDBGrid(FComponent.Components[i]),
  	  	TDBGrid(FComponent.Components[i]).Columns, 'Width');
end;

{ TMDIChildForm }

procedure TMDIChildForm.WMCreate(var Message: TWMCreate);
begin
	inherited;
	FCommonStorage := TMDIChildStorage.Create(Self);
end;

procedure TMDIChildForm.WMDestroy(var Message: TWMDestroy);
begin
	if Assigned(FOnChildDestroy) then
    FOnChildDestroy(Self);

  FCommonStorage.Free;
	inherited;
end;

procedure TMDIChildForm.WMMDIActivate(var Message: TWMMDIActivate);
begin
	inherited;
  if Self.Handle = Message.ActiveWnd then
		if Assigned(FOnMDIActivate) then
  	  FOnMDIActivate(Self);

  if Self.Handle = Message.DeactiveWnd then
		if Assigned(FOnMDIDeactivate) then
  	  FOnMDIDeactivate(Self);
end;

procedure TMDIChildForm.SetCaption(NewCaption: string);
begin
	Caption := NewCaption;
	if OrderNum > 1 then
  	Caption := Format('%s (%d)', [Caption, OrderNum]);
	if Assigned(FOnCaptionChanged) then
  	FOnCaptionChanged(Self);
end;

class function TMDIChildForm.IsCanCreate: boolean;
begin
  Result := true;
end;

procedure TMDIChildForm.WMChar(var Message: TWMChar);
begin
	inherited;
end;

procedure TMDIChildForm.RefreshDataSets;
var
	i: Integer;
begin
	for i := 0 to ComponentCount - 1 do
  	if Components[i] is TCustomADODataSet then
    	if TCustomADODataSet(Components[i]).Active then
		    OpenBigDataSet(TCustomADODataSet(Components[i]));
end;

end.
