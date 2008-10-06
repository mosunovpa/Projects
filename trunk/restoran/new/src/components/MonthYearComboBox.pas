unit MonthYearComboBox;

interface

uses
  Windows, Messages, SysUtils, Classes, Controls, StdCtrls;

type
	TDateCollection = class (TCollection);

	TDateCollectionItem = class (TCollectionItem)
  public
  	FDate: TDate;
  end;

  TMonthYearComboBox = class(TComboBox)
  private
    FDates: TDateCollection;
    FShowDay: Boolean;
    function GetDate: TDateTime;
    procedure SetDate(const Value: TDateTime);
  protected
    { Protected declarations }
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
		procedure Clear; override;
    procedure Fill(BeginDate, EndDate: TDate);
    property Date: TDateTime read GetDate write SetDate;
    property ShowDay: Boolean read FShowDay write FShowDay;
  published
    { Published declarations }
  end;

procedure Register;

implementation

uses
	DateUtils;

procedure Register;
begin
  RegisterComponents('CtrlsEx', [TMonthYearComboBox]);
end;

procedure TMonthYearComboBox.Clear;
begin
  FDates.Clear;
  inherited;
end;

constructor TMonthYearComboBox.Create(AOwner: TComponent);
begin
  inherited;
  Style := csDropDownList;
	FDates := TDateCollection.Create(TDateCollectionItem);
end;

destructor TMonthYearComboBox.Destroy;
begin
  FDates.Free;
  inherited;
end;

procedure TMonthYearComboBox.Fill(BeginDate, EndDate: TDate);
var
	CurDate: TDate;
  Item: TDateCollectionItem;
begin
  if ((BeginDate = 0) and (EndDate = 0)) or (BeginDate > EndDate) then
    Exit;

	BeginDate := StartOfTheMonth(BeginDate);
	EndDate := StartOfTheMonth(EndDate);

  if BeginDate = 0 then
  	BeginDate := EndDate
  else if EndDate = 0 then
  	EndDate := BeginDate;

  Clear;
  CurDate := BeginDate;
  while CurDate <= EndDate do
  begin
	  if ShowDay then
	    AddItem(FormatDateTime('d mmmm yyyy', CurDate), nil)
    else
	    AddItem(FormatDateTime('mmmm yyyy', CurDate), nil);

    Item := TDateCollectionItem(FDates.Add);
    Item.FDate := CurDate;
    CurDate := IncMonth(CurDate);
  end;

end;

function TMonthYearComboBox.GetDate: TDateTime;
begin
	Result := 0;
  if ItemIndex >= 0 then
  	Result := TDateCollectionItem(FDates.Items[ItemIndex]).FDate;
end;

procedure TMonthYearComboBox.SetDate(const Value: TDateTime);
var
	i: Integer;
  Date: TDateTime;
begin
	Date := StartOfTheMonth(Value);
  for i := 0 to FDates.Count - 1 do
  	if TDateCollectionItem(FDates.Items[i]).FDate = Date then
    	ItemIndex := i;
end;

end.
