program apteka;

uses
  Forms,
  FMain in 'FMain.pas' {frmMain},
  Storage in 'Storage.pas',
  SelMedicine in 'SelMedicine.pas' {frmSelMedicine},
  FQuantity in 'FQuantity.pas' {frmQuantity},
  FCount in 'FCount.pas' {frmCount};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TfrmSelMedicine, frmSelMedicine);
  Application.CreateForm(TfrmQuantity, frmQuantity);
  Application.CreateForm(TfrmCount, frmCount);
  Application.Run;
end.
