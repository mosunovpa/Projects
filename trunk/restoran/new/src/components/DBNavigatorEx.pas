unit DBNavigatorEx;

interface

uses
  Windows, Messages, SysUtils, Classes, Controls, ExtCtrls, DBCtrls, ADODB, DB;

type
  TDBNavigatorEx = class(TDBNavigator)
  private
    { Private declarations }
  protected
    { Protected declarations }
  public
     constructor Create(AOwner: TComponent); override;
    procedure BtnClick(Index: TNavigateBtn); override;
  published
    { Published declarations }
  end;

procedure Register;

implementation

uses DBUtils;

procedure Register;
begin
  RegisterComponents('DBCtrlsEx', [TDBNavigatorEx]);
end;

{ TDBNavigatorEx }

procedure TDBNavigatorEx.BtnClick(Index: TNavigateBtn);
var
  SortStr: string;
begin
  SortStr := '';
  if DataSource.DataSet is TCustomADODataSet then
		SortStr := TCustomADODataSet(DataSource.DataSet).Sort;

	if Index = nbRefresh then
	begin
    if not (csDesigning in ComponentState) and Assigned(BeforeAction) then
      BeforeAction(Self, Index);
    if DataSource.DataSet is TCustomADODataSet then
	    OpenBigDataSet(TCustomADODataSet(DataSource.DataSet), SortStr)
    else
    begin
      DataSource.DataSet.Close;
      DataSource.DataSet.Open;
    end;
  end
	else
		  inherited;
end;

constructor TDBNavigatorEx.Create(AOwner: TComponent);
begin
  inherited;
  Hints.Add('������ ������');
  Hints.Add('���������� ������');
  Hints.Add('��������� ������');
  Hints.Add('��������� ������');
  Hints.Add('�������� ������ (Ins)');
  Hints.Add('������� ������ (Ctrl+Del)');
  Hints.Add('������������� ������ (F2)');
  Hints.Add('��������� ��������� (Ctrl+S)');
  Hints.Add('�������� ��������� (Esc)');
  Hints.Add('��������');

	ShowHint := true;
end;

end.
