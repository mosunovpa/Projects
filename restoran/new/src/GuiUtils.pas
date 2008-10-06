unit GuiUtils;

interface

uses
  StdCtrls, DateUtils, Controls, ADODB, DBFilter;

procedure FillMonthCombo(Combo: TComboBox);

type
	TComboFilter = class
  public
  	constructor Create(Combo: TComboBox; DataSet: TCustomADODataSet;
    	FieldName, FieldId: string);
    procedure AssignTo(Filter: TFilter);
  protected
  	FCombo: TComboBox;
  	procedure FillCombo(DataSet: TCustomADODataSet;
    	FieldName, FieldId: string);
  end;

implementation

uses
	SysUtils, Math;

var
	Months: array[1..12] of string = (
		'Январь', 'Февраль', 'Март', 'Апрель', 'Май', 'Июнь',
    'Июль', 'Август', 'Сентябрь', 'Октябрь', 'Ноябрь', 'Декабрь');

procedure FillMonthCombo(Combo: TComboBox);
var
	i: Integer;
begin
	with Combo do
  begin
    Clear;
		for i := 1 to 12 do
    	AddItem(Months[i], TObject(i));
  end;
end;

{ TComboFilter }

constructor TComboFilter.Create(Combo: TComboBox;
  DataSet: TCustomADODataSet; FieldName, FieldId: string);
begin
	FCombo := Combo;
  FillCombo(DataSet, FieldName, FieldId);
end;

procedure TComboFilter.FillCombo(DataSet: TCustomADODataSet; FieldName,
  FieldId: string);
begin
	FCombo.Clear;
	FCombo.Items.AddObject('<Все>', TObject(0));
  DataSet.Open;
  while not DataSet.Eof do
  begin
    FCombo.Items.AddObject(DataSet.FieldByName(FieldName).AsString,
    	TObject(DataSet.FieldByName(FieldId).AsInteger));
  	DataSet.Next;
  end;
  FCombo.ItemIndex := 0;
end;

procedure TComboFilter.AssignTo(Filter: TFilter);
begin
	with FCombo do
  begin
    Filter.FieldValue := Integer(Items.Objects[ItemIndex]);
    if Integer(Items.Objects[ItemIndex]) = 0 then
      Filter.Active := False
    else
      Filter.Active := True;
  end;
end;

end.
