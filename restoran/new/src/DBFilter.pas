unit DBFilter;

interface

uses
	Classes, ADODB, SysUtils;

type
	TFilter = class (TCollectionItem)
  public
  	FieldName: string;
    FieldValue: Variant;
    Active: Boolean;
  end;

	TFilters = class (TCollection)
  public
    function GetFilter(Name: string): TFilter;
    procedure ApplyFilters(DataSet: TCustomADODataSet);
		procedure SetFilteredValuesTo(DataSet: TCustomADODataSet);
    function IsRecVisible(DataSet: TCustomADODataSet): boolean;
  end;

procedure CheckRecFiltered(Filters: TFilters; DataSet: TCustomADODataSet);

implementation

uses
	StrUtil, Controls, Dialogs;

procedure CheckRecFiltered(Filters: TFilters; DataSet: TCustomADODataSet);
begin
  if not Filters.IsRecVisible(DataSet) then
  begin
  	if mrCancel = MessageDlg(
    	'Текущая запись не соответствует условиям фильтра. ' +
      'После редактирования запись будет скрыта. ' +
      'Для просмотра записи снимите фильтр или установите его ' +
      'в соответствующее значение.' + #13#10#13#10 +
      'Сохранить изменения?',
    	mtConfirmation, [mbOK, mbCancel], 0) then
    begin
       Abort;
    end;
  end;
end;

{ TFilters }

procedure TFilters.ApplyFilters(DataSet: TCustomADODataSet);
var
  i: Integer;
  formatter: TFilterFormatter;
begin
	DataSet.Filtered := False;
  DataSet.Filter := '';
  formatter := TFilterFormatter.Create;
  try
    for i := 0 to Count - 1 do
    begin
      if TFilter(Items[i]).Active then
      	formatter.AddFilter(
        	Format('%s = %s',
        		[TFilter(Items[i]).FieldName,
	          TFilter(Items[i]).FieldValue]));
    end;
    DataSet.Filter := formatter.GetString;
    if (DataSet.Filter <> '') then
    	DataSet.Filtered := True;
  finally
    formatter.Free;
  end;

end;

procedure TFilters.SetFilteredValuesTo(DataSet: TCustomADODataSet);
var
  I, J: Integer;
begin
  for I := 0 to Count-1 do
	  if TFilter(Items[I]).Active = True then
	    for J := 0 to DataSet.FieldCount - 1 do
	      if (TFilter(Items[I]).FieldName = DataSet.Fields[J].FieldName) then
        begin
          DataSet.Fields[J].NewValue := TFilter(Items[I]).FieldValue;
        end;
end;

function TFilters.GetFilter(Name: string): TFilter;
var
  I: Integer;
begin
  for I := 0 to Count-1 do
  begin
    Result := TFilter(Items[I]);
    if Result.FieldName = Name then Exit;
  end;
  Result := nil;
end;

function TFilters.IsRecVisible(DataSet: TCustomADODataSet): boolean;
var
	I, J: Integer;
begin
	Result := True;
  for I := 0 to Count-1 do
	  if TFilter(Items[I]).Active = True then
	    for J := 0 to DataSet.FieldCount - 1 do
	      if (TFilter(Items[I]).FieldName = DataSet.Fields[J].FieldName) and
					(TFilter(Items[I]).FieldValue <> DataSet.Fields[J].Value) then
        begin
          Result := False;
          Exit;
        end;
end;

end.
