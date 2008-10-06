object frmSetDate: TfrmSetDate
  Left = 243
  Top = 255
  BorderStyle = bsDialog
  Caption = #1058#1077#1082#1091#1097#1072#1103' '#1076#1072#1090#1072
  ClientHeight = 74
  ClientWidth = 266
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
    Width = 165
    Height = 17
    AutoSize = False
    Caption = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1077' '#1090#1077#1082#1091#1097#1091#1102' '#1076#1072#1090#1091':'
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
    TabOrder = 1
  end
  object Button2: TButton
    Left = 181
    Top = 41
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&'#1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 2
  end
  object calendar: TDateTimePicker
    Left = 8
    Top = 29
    Width = 165
    Height = 21
    CalAlignment = dtaLeft
    Date = 38379.4766573958
    Time = 38379.4766573958
    DateFormat = dfLong
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 0
  end
end
