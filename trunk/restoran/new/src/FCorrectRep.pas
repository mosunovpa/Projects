unit FCorrectRep;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, QuickRpt, DB, ADODB, QRCtrls;

procedure ShowCorrectionsReport(Date: TDateTime);

type
  TfrmCorrectRep = class(TForm)
    QuickRep: TQuickRep;
    qry: TADOQuery;
    DetailBand1: TQRBand;
    QRDBText1: TQRDBText;
    QRDBText2: TQRDBText;
    ColumnHeaderBand1: TQRBand;
    TitleBand1: TQRBand;
    PageFooterBand1: TQRBand;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel3: TQRLabel;
    lblDate: TQRLabel;
    QRSysData1: TQRSysData;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

implementation

uses DMMain, DMRestCalc, Constants;

{$R *.dfm}

procedure ShowCorrectionsReport(Date: TDateTime);
begin
	with TfrmCorrectRep.Create(Application) do
  	try
	    qry.SQL.Clear;	
      qry.SQL.Add(
      	'SELECT Meal.Name, ' +
        	'sum (iif(MealOperations.OperationId = :p0,  -MealOperations.Quantity, ' +
        		'MealOperations.Quantity)) as s ' +
        'FROM Meal INNER JOIN MealOperations ON Meal.Id = MealOperations.MealId ' +
        'where Type = :p1 and MealOperations.OperationDate = :p2 ' +
        'group by Meal.Name ' +
        'ORDER BY Meal.Name');

			qry.Parameters[0].Value := TYPE_CHARGES;
			qry.Parameters[1].Value := SUBTYPE_CORRECTION;
			qry.Parameters[2].Value := Date;

    	qry.Open;
      lblDate.Caption := DateToStr(Date);

	    QuickRep.PreviewModal;
      
    finally
      Free;
    end;
  end;

end.
