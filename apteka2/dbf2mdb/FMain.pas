unit FMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, DB, DBTables, StdCtrls, ADODB;

type
  TfrmMain = class(TForm)
    Button1: TButton;
    DatabaseDbf: TDatabase;
    tblPatient: TTable;
    tblMedicine: TTable;
    tblPatientMedicine: TTable;
    tblSelectedMedicine: TTable;
    ADOConnection1: TADOConnection;
    ADOPatient: TADOTable;
    ADOMedicine: TADOTable;
    ADOPatientMedicine: TADOTable;
    ADOSelectedMedicine: TADOTable;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

	TPair = class(TCollectionItem)
	private
    { Private declarations }
	public
    { Public declarations }
    	FFirst: integer;
        FSecond: integer;
	end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

procedure TfrmMain.Button1Click(Sender: TObject);
var
    FPatColl: TCollection;
    FMedColl: TCollection;
    pair: TPair;
    i: integer;
begin
	DatabaseDbf.Open;
    tblPatient.Open;
    tblMedicine.Open;
    tblPatientMedicine.Open;
    tblSelectedMedicine.Open;
    ADOConnection1.Open;
    ADOPatient.Open;
    ADOMedicine.Open;
    ADOPatientMedicine.Open;
    ADOSelectedMedicine.Open;

    FPatColl := TCollection.Create(TPair);
    FMedColl := TCollection.Create(TPair);
    try
       while not tblPatient.Eof do
        begin
        	ADOPatient.Insert;
            for i := 1 to tblPatient.Fields.Count - 1 do
            begin
	            ADOPatient.Fields[i].Value := tblPatient.Fields[i].Value;
            end;
            ADOPatient.Post;

        	pair := FPatColl.Add as TPair;
            pair.FFirst := tblPatient.Fields[0].AsInteger;
            pair.FSecond := ADOPatient.Fields[0].AsInteger;

        	tblPatient.Next;
        end;
        while not tblMedicine.Eof do
        begin
        	ADOMedicine.Insert;
            for i := 1 to tblMedicine.Fields.Count - 1 do
            begin
	            ADOMedicine.Fields[i].Value := tblMedicine.Fields[i].Value;
            end;
            ADOMedicine.Post;

        	pair := FMedColl.Add as TPair;
            pair.FFirst := tblMedicine.Fields[0].AsInteger;
            pair.FSecond := ADOMedicine.Fields[0].AsInteger;

        	tblMedicine.Next;
        end;
        while not tblPatientMedicine.Eof do
        begin
            ADOPatientMedicine.Insert;
            for i := 0 to FPatColl.Count - 1 do
            begin
                if (FPatColl.Items[i] as TPair).FFirst = tblPatientMedicine.Fields[0].AsInteger then
                begin
                    ADOPatientMedicine.Fields[0].AsInteger := (FPatColl.Items[i] as TPair).FSecond;
                    Break;
                end;
            end;
            for i := 0 to FMedColl.Count - 1 do
            begin
                if (FMedColl.Items[i] as TPair).FFirst = tblPatientMedicine.Fields[1].AsInteger then
                begin
                    ADOPatientMedicine.Fields[1].AsInteger := (FMedColl.Items[i] as TPair).FSecond;
                    Break;
                end;
            end;
            for i := 2 to tblPatientMedicine.Fields.Count - 1 do
            begin
                ADOPatientMedicine.Fields[i].Value := tblPatientMedicine.Fields[i].Value;
            end;
            if (ADOPatientMedicine.Fields[0].AsInteger = 0) or
            	(ADOPatientMedicine.Fields[1].AsInteger = 0) then
	            ADOPatientMedicine.Cancel
            else
	            ADOPatientMedicine.Post;

            tblPatientMedicine.Next;
        end;

    finally
        FPatColl.Free;
        FMedColl.Free;
    end;
    ShowMessage('done');
end;

end.
