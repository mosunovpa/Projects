object frmSetMonth: TfrmSetMonth
  Left = 216
  Top = 154
  BorderStyle = bsDialog
  Caption = #1044#1072#1090#1072' '#1091#1076#1072#1083#1077#1085#1080#1103
  ClientHeight = 75
  ClientWidth = 267
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object lblTitle: TLabel
    Left = 8
    Top = 8
    Width = 165
    Height = 29
    AutoSize = False
    Caption = #1059#1082#1072#1078#1080#1090#1077' '#1076#1072#1090#1091' '#1076#1086' '#1082#1086#1090#1086#1088#1086#1081' '#1073#1091#1076#1091#1090' '#1091#1076#1072#1083#1077#1085#1099' '#1074#1089#1077' '#1076#1072#1085#1085#1099#1077':'
    WordWrap = True
  end
  object Button1: TButton
    Left = 181
    Top = 9
    Width = 75
    Height = 25
    Caption = #1054'&'#1050
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object Button2: TButton
    Left = 181
    Top = 41
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&'#1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 1
  end
  object comboMonth: TMonthYearComboBox
    Left = 8
    Top = 40
    Width = 165
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 2
  end
end
