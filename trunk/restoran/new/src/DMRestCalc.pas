unit DMRestCalc;

interface

uses
  SysUtils, Classes, Controls, DB, ADODB, Constants;

procedure SetParameterValues(Query: TADOQuery; Name: string; Value: Variant);
function QueryValue(qry: TADOQuery; sqlStr: string): Variant;
function QueryValue2(qry: TADOQuery; sqlStr: string;
	var FieldVal1: Variant; var FieldVal2: Variant): boolean;
procedure QueryExecute(qry: TADOQuery; sqlStr: string;
	const Args: array of const);
function GetMovingSql(ParamName1, ParamName2: string): string;
function GetMovingExSql(
	ParamName1, ParamName2, ParamName3, ParamName4: string): string;
function ConstructSelectMovingSql(qryMoving: TADOQuery; Date: TDateTime): string;
function ConstructSelectMovingExSql(qryMoving: TADOQuery;
	BeginDate, EndDate: TDateTime): string;

type
  TdmodRestCalc = class(TDataModule)
    qryRest: TADOQuery;
    qry: TADOQuery;
  private
		procedure ExecRestDel(Date: TDateTime);
  	procedure ExecRestCalc(Date: TDateTime);
  public
	  function GetMinOperationDate: TDateTime;
	  function GetMaxOperationDate: TDateTime;
	  function GetMinStorageDate: TDateTime;
	  function GetMaxStorageDate: TDateTime;
	  function GetMinDate: TDateTime;
	  function GetMaxDate: TDateTime;

	  function GetMaxMaxOperationDate(
    	var Field1, Field2: TDateTime): boolean;
		function GetMinMaxStorageDate(
    	var Field1, Field2: TDateTime): boolean;

    procedure CalculateRest(BeginDate, EndDate: TDateTime); overload;
    procedure CalculateRest(Date: TDateTime); overload;
    procedure CorrectRest(ProdId: Integer;
    	RestDate: TDateTime; CorrectValue: Currency);
	procedure DeleteData(DelDate: TDateTime);
  end;

var
  dmodRestCalc: TdmodRestCalc;

implementation

{$R *.dfm}

uses
	DateUtils, DMMain, Variants, Math;

const INSERT_MEAL_STORAGE_CLAUSE: string =
		'insert into MealStorage ([Date], MealId, [Quantity]) %s ';
const INSERT_MEAL_STORAGE_FIELDS: string =
    '    #%s# as [Date], Meal.Id as MealId,                            '+
    '    iif(Storages.Storage is Null, 0, Storages.Storage) +          '+
    '		 iif(Receipts.Receipt is Null, 0, Receipts.Receipt) -          '+
    '    iif(Charges.Charge is Null, 0, Charges.Charge) -              '+
    '		 iif(DishCharges.DishCharge is Null, 0, DishCharges.DishCharge)'+
    '		 as Quantity          																				 ';

const SELECT_MOVING_FIELDS: string =
    '    Meal.Id, Meal.Name,                                           '+
    '    iif(Storages.Storage is Null, 0, Storages.Storage) as Storage,					 '+
    '		 iif(Receipts.Receipt is Null, 0, Receipts.Receipt) as Receipt,           '+
    '    iif(Charges.Charge is Null, 0, Charges.Charge) as Charge,  	 		  		 '+
    '		 iif(DishCharges.DishCharge is Null, 0, DishCharges.DishCharge) as DishCharge ';

const SELECT_MOVING_EX_FIELDS: string =
    '    Meal.Id, Meal.Name,                                           '+
    '    iif(Storages.Storage is Null, 0, Storages.Storage) as Storage,					 '+
    '		 iif(Receipts.Receipt is Null, 0, Receipts.Receipt) as Receipt,           '+
    '    iif(Charges.Charge is Null, 0, Charges.Charge) as Charge,  	 		  		 '+
    '		 iif(DishCharges.DishCharge is Null, 0, DishCharges.DishCharge) as DishCharge, '+
    '		 iif(ReceiptsEnd.Receipt is Null, 0, ReceiptsEnd.Receipt) as ReceiptEnd,           '+
    '    iif(ChargesEnd.Charge is Null, 0, ChargesEnd.Charge) as ChargeEnd,  	 		  		 '+
    '		 iif(DishChargesEnd.DishCharge is Null, 0, DishChargesEnd.DishCharge) as DishChargeEnd ';

const SELECT_MOVING : string =
    '  select %s                                          '+
    '  from                                               '+
    '  (                                                  '+
    '    (                                                '+
    '      (                                              '+
    '        Meal                                         '+
    '        left join                                    '+
    '        (                                            '+
    '        	%s                                          '+
    '        ) as Storages                                '+
    '        on Meal.Id = Storages.MealId                 '+
    '      )                                              '+
    '      left join                                      '+
    '      (                                              '+
    '        	%s                                          '+
    '      ) as Receipts                                  '+
    '      on Meal.Id = Receipts.MealId                   '+
    '    )                                                '+
    '    left join                                        '+
    '    (                                                '+
    '      %s                                             '+
    '    ) as Charges                                     '+
    '    on Meal.Id = Charges.MealId                      '+
    '  )                                                  '+
    '  left join                                          '+
    '  (                                                  '+
    '   	%s                                              '+
    '  ) as DishCharges                                   '+
    '  on Meal.Id = DishCharges.MealId                    ';

const SELECT_MOVING_EX : string =
    '  select %s                                          '+
    '  from                                               '+
    '  (																								 	'+
    '    (																							 	'+
    '      (                                              '+
    '        (                                            '+
    '          (                                          '+
    '            (                                        '+
    '              Meal                           				'+
    '              left join                              '+
    '              (                                      '+
    '              	%s                                    '+
    '              ) as Storages                          '+
    '              on Meal.Id = Storages.MealId           '+
    '            )                                        '+
    '            left join                                '+
    '            (                                        '+
    '              	%s                                    '+
    '            ) as Receipts                            '+
    '            on Meal.Id = Receipts.MealId             '+
    '          )                                          '+
    '          left join                                  '+
    '          (                                          '+
    '            %s     	                                '+
    '          ) as Charges                               '+
    '          on Meal.Id = Charges.MealId                '+
    '        )                                            '+
    '        left join                                    '+
    '        (                                            '+
    '         	%s                                        '+
    '        ) as DishCharges                             '+
    '        on Meal.Id = DishCharges.MealId              '+
    '      )                                              '+
    '      left join                                      '+
    '      (                                              '+
    '        	%s                                          '+
    '      ) as ReceiptsEnd                               '+
    '      on Meal.Id = ReceiptsEnd.MealId                '+
    '    )																								'+
    '    left join                                        '+
    '    (                                                '+
    '      %s     	                                      '+
    '    ) as ChargesEnd                                  '+
    '    on Meal.Id = ChargesEnd.MealId                   '+
    '  )                                                  '+
    '  left join                                          '+
    '  (                                                  '+
    '   	%s                                              '+
    '  ) as DishChargesEnd                                '+
    '  on Meal.Id = DishChargesEnd.MealId                 ';


const SELECT_MOVING_ORDER_CLAUSE: string =
		' order by Name ';

procedure SaveToFile(s: string);
var
	fl: Text;
begin
  AssignFile(fl, 'C:\Temp\log.txt');
  Append(fl);
  Writeln(fl, s);
  CloseFile(fl);
end;

procedure SetParameterValues(Query: TADOQuery; Name: string;
  Value: Variant);
var
	i: Integer;
begin
	with Query.Parameters do
    for i := 0 to Count - 1 do
      if Items[i].Name = Name then
      	Items[i].Value := Value;
end;

function QueryValue(qry: TADOQuery; sqlStr: string): Variant;
begin
	Result := Null;
	with qry do
  begin
  	SQL.Clear;
    SQL.Add(sqlStr);
    Open;
    try
      if not Eof then
        Result := Fields[0].Value;
    finally
	    Close;
    end;
  end;
end;

function QueryValue2(qry: TADOQuery; sqlStr: string;
	var FieldVal1: Variant; var FieldVal2: Variant): boolean;
begin
	Result := false;
	with qry do
  begin
  	SQL.Clear;
    SQL.Add(sqlStr);
    Open;
    First;
    try
      if not Eof then
      begin
        FieldVal1 := Fields[0].Value;
        FieldVal2 := Fields[1].Value;
        Result := true;
      end;
		finally
	    Close;
    end;
  end;
end;

procedure QueryExecute(qry: TADOQuery; sqlStr: string;
	const Args: array of const);
var
	i: Integer;
  param: Variant;
begin
	with qry do
  begin
  	Close;
  	SQL.Clear;
    SQL.Add(sqlStr);
	  Parameters.ParseSQL(SQL.Text, true);
   	for i := 0 to High(Args) do
    begin
      with Args[I] do
      begin
        case VType of
          vtInteger:    param := VInteger;
          vtBoolean:    param := VBoolean;
          vtChar:       param := VChar;
          vtExtended:   param := VExtended^;
          vtString:     param := VString^;
          vtPChar:      param := string(VPChar);
          vtObject:     param := VObject.ClassName;
          vtClass:      param := VClass.ClassName;
          vtAnsiString: param := string(VAnsiString);
          vtCurrency:   param := VCurrency^;
          vtVariant:    param := VVariant^;
          vtInt64:      param := VInt64^;
	     	end;
      end;
      Parameters[i].Value := param;
    end;
	  ExecSql;
	  Close;
  end;
end;

function GetStoragesSelect(ParamName: string): string;
begin
  Result :=
    ' select MealId, Quantity as Storage from MealStorage '+
    ' where Date = :'+ ParamName + ' ';
end;

function GetReceiptsSelect(ParamName1, ParamName2: string): string;
begin
	Result := Format(
    ' select MealId, sum(Quantity) as Receipt   '+
    ' from                                      '+
    ' MealOperations                            '+
    ' where OperationId = 1                     '+
    '   and MealOperations.OperationDate >= :%s '+
    '   and MealOperations.OperationDate <= :%s '+
    ' group by MealId                           ',
    [ParamName1, ParamName2]);
end;

function GetChargesSelect(ParamName1, ParamName2: string): string;
begin
	Result := Format(
    ' select MealId, sum(Quantity) as Charge     '+
    ' from MealOperations                        '+
    ' where OperationId = 2                      '+
    '   and MealOperations.OperationDate >= :%s  '+
    '   and MealOperations.OperationDate <= :%s  '+
    ' group by MealId                            ',
    [ParamName1, ParamName2]);
end;

function GetDishChargesSelect(ParamName1, ParamName2: string): string;
begin
	Result := Format(
    ' select SoldDishComponents.MealId,                                         '+
    '   sum(DishesSelling.Count * SoldDishComponents.Quantity) as DishCharge    '+
    ' from DishesSelling                                                    '+
    ' left join SoldDishComponents                            '+
    ' on DishesSelling.Id = SoldDishComponents.SoldDishId     '+
    ' where                                               '+
    '		 DishesSelling.SelDate >= :%s                   	 '+
    '   and DishesSelling.SelDate <= :%s                  '+
    ' group by SoldDishComponents.MealId                      ',
    [ParamName1, ParamName2]);
end;

function GetInsSqlStr(Date: TDateTime; ParamName1, ParamName2: string): string;
var
	sDate: string;
begin
	sDate := Format('%d/%d/%d', [MonthOf(Date), DayOf(Date), YearOf(Date)]);
  Result := Format(INSERT_MEAL_STORAGE_CLAUSE,
  	[
    	Format(SELECT_MOVING,
    	[
      	Format(INSERT_MEAL_STORAGE_FIELDS, [sDate]),
        GetStoragesSelect(ParamName1),
        GetReceiptsSelect(ParamName1, ParamName2),
        GetChargesSelect(ParamName1, ParamName2),
        GetDishChargesSelect(ParamName1, ParamName2)
      ])
    ]);
end;

function GetMovingSql(ParamName1, ParamName2: string): string;
begin
  Result := Format(SELECT_MOVING,
  	[
    	SELECT_MOVING_FIELDS,
      GetStoragesSelect(ParamName1),
      GetReceiptsSelect(ParamName1, ParamName2),
      GetChargesSelect(ParamName1, ParamName2),
      GetDishChargesSelect(ParamName1, ParamName2)
    ]);
  Result := Result + SELECT_MOVING_ORDER_CLAUSE;
end;

function GetMovingExSql(
	ParamName1, ParamName2, ParamName3, ParamName4: string): string;
begin
	SetLength(Result, 6000);

	StrFmt(PChar(Result), PChar(SELECT_MOVING_EX),
  	[
    	SELECT_MOVING_EX_FIELDS,
      GetStoragesSelect(ParamName1),
      GetReceiptsSelect(ParamName1, ParamName2),
      GetChargesSelect(ParamName1, ParamName2),
      GetDishChargesSelect(ParamName1, ParamName2),
      GetReceiptsSelect(ParamName3, ParamName4),
      GetChargesSelect(ParamName3, ParamName4),
      GetDishChargesSelect(ParamName3, ParamName4)
    ]);
	StrCopy(PChar(Result) + StrLen(PChar(Result)),
  	PChar(' ' + SELECT_MOVING_ORDER_CLAUSE));
end;

function ConstructSelectMovingSql(qryMoving: TADOQuery; Date: TDateTime): string;
begin
  qryMoving.Sql.Clear;
  qryMoving.Sql.Add(GetMovingSql('pBeginDate', 'pEndDate'));

	// create parameters list
  qryMoving.Parameters.ParseSQL(qryMoving.SQL.Text, true);
  SetParameterValues(qryMoving, 'pBeginDate', StartOfTheMonth(Date));
  SetParameterValues(qryMoving, 'pEndDate', Floor(Date));
end;

function ConstructSelectMovingExSql(qryMoving: TADOQuery;
	BeginDate, EndDate: TDateTime): string;
begin
  qryMoving.Sql.Clear;
  qryMoving.Sql.Add(GetMovingExSql('pBeginDate', 'pEndDate', 'pBeginDate2', 'pEndDate2'));

	// create parameters list
  qryMoving.Parameters.ParseSQL(qryMoving.SQL.Text, true);
  SetParameterValues(qryMoving, 'pBeginDate', StartOfTheMonth(BeginDate));
  SetParameterValues(qryMoving, 'pEndDate', Floor(IncDay(BeginDate, -1)));
  SetParameterValues(qryMoving, 'pBeginDate2', Floor(BeginDate));
  SetParameterValues(qryMoving, 'pEndDate2', Floor(EndDate));
end;

{ TdmodRestCalc }

procedure TdmodRestCalc.CalculateRest(BeginDate, EndDate: TDateTime);
var
	CurDate: TDateTime;
begin
  if EndDate >= BeginDate then
  begin
    CurDate := BeginDate;
    while CurDate <= EndDate do
    begin
      CalculateRest(CurDate);
    	CurDate := IncMonth(CurDate);
    end;
  end;
end;

procedure TdmodRestCalc.CalculateRest(Date: TDateTime);
begin
	ExecRestDel(Date);
  ExecRestCalc(Date);
end;

procedure TdmodRestCalc.ExecRestDel(Date: TDateTime);
begin
	Date := StartOfTheMonth(Date); // first day of the month
  qryRest.Close;
	qryRest.SQL.Clear;
  qryRest.SQL.Add('delete from MealStorage where Date = :pDelDate');
  qryRest.Parameters.ParseSQL(qryRest.SQL.Text, true);
  SetParameterValues(qryRest, 'pDelDate', Date);
  qryRest.ExecSQL;
  qryRest.Close;
end;

procedure TdmodRestCalc.ExecRestCalc(Date: TDateTime);
var
	BeginDate, EndDate: TDateTime;
begin
	EndDate := IncDay(Date, -1); // last day of the previous month
  BeginDate := StartOfTheMonth(EndDate); // first day of the previous month
  qryRest.Close;
	qryRest.SQL.Clear;
	qryRest.SQL.Add(
  	GetInsSqlStr(Date, 'pBeginDate', 'pEndDate') // Date - date of the rests
    );
	// create parameters list
  qryRest.Parameters.ParseSQL(qryRest.SQL.Text, true);

  SetParameterValues(qryRest, 'pBeginDate', BeginDate);
  SetParameterValues(qryRest, 'pEndDate', EndDate);

 	qryRest.ExecSQL;
 	qryRest.Close;
end;

function TdmodRestCalc.GetMaxOperationDate: TDateTime;
var
	Date: Variant;
begin
 	Result := 0;
  // get max operation date
  Date := QueryValue(qry, 'select max(OperationDate) from MealOperations');
  if (not VarIsNull(Date)) then
  	Result := Date;

  // get max selling dishes date
  Date := QueryValue(qry, 'select max(SelDate) from DishesSelling');
  if (not VarIsNull(Date)) and (Date > Result) then
  	Result := Date;
end;

function TdmodRestCalc.GetMinOperationDate: TDateTime;
var
	Date: Variant;
begin
 	Result := 0;
  // get min operation date
  Date := QueryValue(qry, 'select min(OperationDate) from MealOperations');
  if (not VarIsNull(Date)) then
  	Result := Date;

  // get min selling dishes date
  Date := QueryValue(qry, 'select min(SelDate) from DishesSelling');
  if (not VarIsNull(Date)) and (Date < Result) then
  	Result := Date;
end;

function TdmodRestCalc.GetMaxStorageDate: TDateTime;
var
	Date: Variant;
begin
	Result := 0;
  Date := QueryValue(qry, 'select max(Date) from MealStorage');
  if (not VarIsNull(Date)) then
  	Result := Date;
end;

function TdmodRestCalc.GetMinStorageDate: TDateTime;
var
	Date: Variant;
begin
	Result := 0;
  Date := QueryValue(qry, 'select min(Date) from MealStorage');
  if (not VarIsNull(Date)) then
  	Result := Date;
end;

function TdmodRestCalc.GetMaxDate: TDateTime;
var
	Date: TDateTime;
begin
  Result := GetMaxStorageDate;

  Date := GetMaxOperationDate;
  if Date > Result then
  	Result := Date;
end;

function TdmodRestCalc.GetMinDate: TDateTime;
var
	Date: TDateTime;
begin
  Result := GetMinStorageDate;

  Date := GetMinOperationDate;
  if Date < Result then
  	Result := Date;
end;

function TdmodRestCalc.GetMaxMaxOperationDate(var Field1,
  Field2: TDateTime): boolean;
var
	F1, F2: Variant;
  res1, res2: boolean;
begin
  res1 := QueryValue2(qry,
  	'select min(OperationDate), max(OperationDate) from MealOperations',
    F1, F2);
  if VarIsNull(F1) then
  	res1 := false;

  if res1 = true then
  begin
  	Field1 := F1;
  	Field2 := F2;
  end;

  res2 := QueryValue2(qry,
  	'select min(SelDate), max(SelDate) from DishesSelling',
    F1, F2);

  if VarIsNull(F1) then
  	res2 := false;

  if res2 = true then
  begin
  	if F1 < Field1 then
	  	Field1 := F1;
    if F2 > Field2 then
	  	Field2 := F2;
  end;
  Result := res1 or res2;
end;

function TdmodRestCalc.GetMinMaxStorageDate(var Field1,
  Field2: TDateTime): boolean;
var
	F1, F2: Variant;
begin
  Result := QueryValue2(qry,
  	'select min(Date), max(Date) from MealStorage',
    F1, F2);

  if VarIsNull(F1) then
  	Result := false;

  if Result = true then
  begin
    Field1 := F1;
    Field2 := F2;
  end;
end;

procedure TdmodRestCalc.CorrectRest(ProdId: Integer;
  RestDate: TDateTime; CorrectValue: Currency);
var
  OperationId: Integer;
begin
	if CorrectValue = 0 then
  	Exit;

  if CorrectValue < 0 then
  begin
  	CorrectValue := -CorrectValue;
    OperationId := TYPE_CHARGES;
  end
  else
    OperationId := TYPE_RECEIPTS;

	with qry do
  begin
    SQL.Clear;
    SQL.Add('insert into MealOperations');
    SQL.Add('(OperationDate, MealId, Quantity, OperationId, Type)');
    SQL.Add('values (:p1, :p2, :p3, :p4, :p5)');
    Parameters.ParseSQL(qry.SQL.Text, true);
    Parameters[0].Value := RestDate;
    Parameters[1].Value := ProdId;
    Parameters[2].Value := CorrectValue;
    Parameters[3].Value := OperationId;
    Parameters[4].Value := SUBTYPE_CORRECTION; // Correction operation
    ExecSql();
    Close;
  end;
end;

procedure TdmodRestCalc.DeleteData(DelDate: TDateTime);
begin
	// delete sood dishes and its components
	QueryExecute(qry,
		'delete from SoldDishComponents where SoldDishId in ' +
    '(select id from DishesSelling where SelDate < :p)',
	  [DelDate]);
	QueryExecute(qry,
		'delete from DishesSelling where SelDate < :p',
	  [DelDate]);

	// delete month rests
	QueryExecute(qry,
		'delete from MealStorage where Date < :p',
	  [DelDate]);

  // delete meal operations
	QueryExecute(qry,
		'delete from MealOperations where OperationDate < :p',
	  [DelDate]);
end;

end.
