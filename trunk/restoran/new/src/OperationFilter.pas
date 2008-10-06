unit OperationFilter;

interface

uses
	StdCtrls, DBFilter;

type
	TOperationFilter = class
  public
  	constructor Create(Combo: TComboBox);
    procedure AssignTo(OperationFilter, TypeFilter: TFilter);
  protected
  	FCombo: TComboBox;
  	procedure FillCombo();
  end;

implementation

uses
	DMRestCalc, Constants;

{ TOperationFilter }

constructor TOperationFilter.Create(Combo: TComboBox);
begin
	FCombo := Combo;
  FillCombo();
end;

procedure TOperationFilter.FillCombo;
begin
	FCombo.Items.AddObject('<Все>', TObject(0));
  FCombo.Items.AddObject('Приходы', TObject(TYPE_RECEIPTS));
  FCombo.Items.AddObject('Расходы', TObject(TYPE_CHARGES));
  FCombo.Items.AddObject('Корректировки', TObject(SUBTYPE_CORRECTION));
  FCombo.ItemIndex := 0;
end;

procedure TOperationFilter.AssignTo(OperationFilter, TypeFilter: TFilter);
var
	Id: Integer;
begin
	with FCombo do
		Id := Integer(Items.Objects[ItemIndex]);

  if Id = 0 then
  begin
    TypeFilter.Active := False;
    OperationFilter.Active := False;
  end
  else
  begin
    if (Id = SUBTYPE_CORRECTION) then
    begin
      OperationFilter.Active := False;
      TypeFilter.Active := True;
      TypeFilter.FieldValue := Id;
    end
    else
    begin
      TypeFilter.Active := False;
      OperationFilter.Active := True;
      OperationFilter.FieldValue := Id;
    end;
  end;
end;

end.
