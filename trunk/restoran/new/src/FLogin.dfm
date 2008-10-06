object frmLogin: TfrmLogin
  Left = 194
  Top = 115
  BorderStyle = bsDialog
  Caption = #1051#1086#1075#1080#1085
  ClientHeight = 107
  ClientWidth = 288
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
  object Label1: TLabel
    Left = 12
    Top = 16
    Width = 99
    Height = 13
    Caption = #1048#1084#1103' '#1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1103':'
  end
  object Label2: TLabel
    Left = 12
    Top = 44
    Width = 41
    Height = 13
    Caption = #1055#1072#1088#1086#1083#1100':'
  end
  object editName: TEdit
    Left = 124
    Top = 12
    Width = 153
    Height = 21
    TabOrder = 0
  end
  object editPassword: TEdit
    Left = 124
    Top = 40
    Width = 153
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
  end
  object btnOk: TButton
    Left = 124
    Top = 76
    Width = 75
    Height = 25
    Caption = #1054'&'#1050
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOkClick
  end
  object Button2: TButton
    Left = 204
    Top = 76
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&'#1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 3
  end
  object qryLogin: TADOQuery
    Connection = dmodMain.connDB
    Parameters = <
      item
        Name = 'p1'
        Attributes = [paNullable]
        DataType = ftFixedChar
        NumericScale = 255
        Precision = 255
        Size = 510
        Value = Null
      end
      item
        Name = 'p2'
        Attributes = [paNullable]
        DataType = ftFixedChar
        NumericScale = 255
        Precision = 255
        Size = 510
        Value = Null
      end>
    SQL.Strings = (
      'select id from users where name = :p1 and password = :p2')
    Left = 20
    Top = 64
  end
end
