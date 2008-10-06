unit FQuantity;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, ComCtrls;

type
  TfrmQuantity = class(TForm)
    Label1: TLabel;
    Edit1: TEdit;
    updwQuantity: TUpDown;
    Bevel1: TBevel;
    Button1: TButton;
    Button2: TButton;
    Label2: TLabel;
    edDescription: TEdit;
    Label3: TLabel;
    edDosa: TEdit;
    Label4: TLabel;
    lblPatient: TLabel;
    Label6: TLabel;
    lblMedicine: TLabel;
    Bevel2: TBevel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmQuantity: TfrmQuantity;

implementation

{$R *.DFM}


end.
