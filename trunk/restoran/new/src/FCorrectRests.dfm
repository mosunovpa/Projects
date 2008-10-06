object frmCorrectRests: TfrmCorrectRests
  Left = 219
  Top = 117
  BorderStyle = bsDialog
  Caption = #1050#1086#1088#1088#1077#1082#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1089#1090#1072#1090#1082#1080
  ClientHeight = 153
  ClientWidth = 239
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
  object lblTitle: TLabel
    Left = 8
    Top = 8
    Width = 223
    Height = 26
    Caption = #1050#1086#1088#1088#1077#1082#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1089#1090#1072#1090#1082#1080' '#1085#1072' %s '#1087#1086' '#1087#1088#1086#1076#1091#1082#1090#1091' "%s"'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 8
    Top = 60
    Width = 111
    Height = 13
    Caption = #1058#1077#1082#1091#1097#1080#1077' '#1086#1089#1090#1072#1090#1082#1080' ('#1075#1088'):'
  end
  object Label3: TLabel
    Left = 8
    Top = 88
    Width = 100
    Height = 13
    Caption = #1053#1086#1074#1099#1077' '#1086#1089#1090#1072#1090#1082#1080' ('#1075#1088'):'
  end
  object editCurrentRests: TEdit
    Left = 136
    Top = 56
    Width = 93
    Height = 21
    TabStop = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 0
  end
  object editNewRests: TEdit
    Left = 136
    Top = 84
    Width = 93
    Height = 21
    TabOrder = 1
  end
  object btnCancel: TButton
    Left = 156
    Top = 120
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&'#1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 3
  end
  object btnOk: TButton
    Left = 76
    Top = 120
    Width = 75
    Height = 25
    Caption = #1054'&'#1050
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOkClick
  end
end
