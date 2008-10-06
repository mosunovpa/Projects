unit FSetMonth;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, MonthYearComboBox;

type
  TfrmSetMonth = class(TForm)
    Button1: TButton;
    Button2: TButton;
    lblTitle: TLabel;
    comboMonth: TMonthYearComboBox;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

implementation

{$R *.dfm}

end.
