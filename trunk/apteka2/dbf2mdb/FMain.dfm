object frmMain: TfrmMain
  Left = 312
  Top = 296
  BorderStyle = bsDialog
  Caption = 'dbf2mdb'
  ClientHeight = 216
  ClientWidth = 471
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 192
    Top = 184
    Width = 75
    Height = 25
    Caption = 'Convert'
    ModalResult = 1
    TabOrder = 0
    OnClick = Button1Click
  end
  object DatabaseDbf: TDatabase
    DatabaseName = 'Dbf'
    DriverName = 'STANDARD'
    LoginPrompt = False
    Params.Strings = (
      'PATH=c:\apteka\data\'
      'DEFAULT DRIVER=PARADOX'
      'ENABLE BCD=FALSE')
    SessionName = 'Default'
    Left = 24
    Top = 16
  end
  object tblPatient: TTable
    DatabaseName = 'Dbf'
    TableName = 'patient.DB'
    Left = 24
    Top = 64
  end
  object tblMedicine: TTable
    DatabaseName = 'Dbf'
    TableName = 'MEDICINE.DB'
    Left = 24
    Top = 112
  end
  object tblPatientMedicine: TTable
    DatabaseName = 'Dbf'
    TableName = 'patient_medicine.DB'
    Left = 120
    Top = 64
  end
  object tblSelectedMedicine: TTable
    DatabaseName = 'Dbf'
    TableName = 'selected_medicine.DB'
    Left = 120
    Top = 112
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=c:\projects\apteka2' +
      '\dbf2mdb\database.mdb;Persist Security Info=False'
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 264
    Top = 16
  end
  object ADOPatient: TADOTable
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = 'patient'
    Left = 264
    Top = 64
  end
  object ADOMedicine: TADOTable
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = 'medicine'
    Left = 264
    Top = 112
  end
  object ADOPatientMedicine: TADOTable
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = 'patient_medicine'
    Left = 384
    Top = 64
  end
  object ADOSelectedMedicine: TADOTable
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = 'selected_medicine'
    Left = 384
    Top = 120
  end
end
