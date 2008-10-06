unit DBGridEx;

interface

uses
  Windows, Messages, SysUtils, Classes, Controls, Grids, DBGrids;

type
  TDBGridEx = class(TDBGrid)
  private
    { Private declarations }
  protected
    { Protected declarations }
  public
    procedure KeyUp(var Key: Word; Shift: TShiftState); override;
  published
    { Published declarations }
  end;

procedure Register;

implementation

uses
	Dialogs, DB;

procedure Register;
begin
  RegisterComponents('DBCtrlsEx', [TDBGridEx]);
end;

{ TDBGridEx }

procedure TDBGridEx.KeyUp(var Key: Word; Shift: TShiftState);
begin
  inherited;
  if (ssCtrl in Shift) and (Key = Ord('S')) then
    if Assigned(DataSource) then
      if Assigned(DataSource.DataSet) then
        if (DataSource.DataSet.State = dsEdit)
          or (DataSource.DataSet.State = dsInsert) then
        begin
          DataSource.DataSet.Post;
        end;
end;

end.
