unit DBUtils;

interface

uses
  Forms, Controls, DB, ADODB;

procedure OpenBigDataSet(ds: TCustomADODataSet; SortStr: string = '');
function GetNullField(ds: TCustomADODataSet): Integer;
procedure CheckNullField(ds: TCustomADODataSet);
procedure ProcessDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);

implementation

uses
	Variants, Dialogs, SysUtils;

procedure OpenBigDataSet(ds: TCustomADODataSet; SortStr: string);
var
  Save_Cursor: TCursor;
  SavedParams: TParams;
  i: Integer;
  Bookmark: TBookmarkStr;
begin
  Bookmark := '';
  if ds.Active then
  	Bookmark := ds.Bookmark;

	if SortStr = '' then
  	SortStr := ds.Sort;
        
  Save_Cursor := Screen.Cursor;
	try
  	Screen.Cursor := crHourGlass;
    if ds is TADOQuery then
    begin
      SavedParams := TParams.Create;
      try
        SavedParams.Assign(TADOQuery(ds).Parameters); // save parameters values
	    	ds.Close;
        TADOQuery(ds).Parameters.ParseSQL(TADOQuery(ds).SQL.Text, true);
        for i := 0 to SavedParams.Count - 1 do // restore parameters values
        	TADOQuery(ds).Parameters[i].Value := SavedParams[i].Value;
  	  	ds.Open;
      finally
        SavedParams.Free;
      end;
    end
    else
    begin
	    ds.Close;
  	  ds.Open;
    end;
    if SortStr <> '' then
      ds.Sort := SortStr;

    if Bookmark <> '' then
      try
	    	ds.Bookmark := Bookmark;
      except
      end
  finally
	  Screen.Cursor := Save_Cursor;
  end
end;

function GetNullField(ds: TCustomADODataSet): Integer;
var
	i: Integer;
begin
	Result := -1;
	for i := 0 to ds.FieldCount - 1 do
  	if ds.Fields[i].Visible and
    	((ds.Fields[i].Value = Null)
      	or (Trim(ds.Fields[i].Value) = '')
      )
    then
    begin
      Result := i;
      Exit;
    end;
end;

procedure CheckNullField(ds: TCustomADODataSet);
var
	i: Integer;
begin
	i := GetNullField(ds);
  if i >= 0 then
  begin
  	MessageDlg('Поле "' +
    	ds.Fields[i].DisplayLabel + '" не может быть пустым.',
      mtError, [mbOK], 0);
    Abort;
  end;
end;

procedure ProcessDeleteError(DataSet: TDataSet;
  E: EDatabaseError; var Action: TDataAction);
begin
	MessageDlg(
  	'Ошибка удаления записи, возможно есть подчинённые записи.'+ #13#10 +
    E.Message,
   	mtError, [mbOK], 0);
	Action := daAbort;
end;

end.
