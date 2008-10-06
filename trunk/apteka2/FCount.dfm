object frmCount: TfrmCount
  Left = 753
  Top = 185
  BorderStyle = bsDialog
  Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1091#1087#1072#1082#1086#1074#1086#1082
  ClientHeight = 89
  ClientWidth = 271
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 11
    Top = 20
    Width = 156
    Height = 13
    Caption = #1059#1082#1072#1078#1080#1090#1077' '#1082#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1091#1087#1072#1082#1086#1074#1086#1082
  end
  object Bevel1: TBevel
    Left = 8
    Top = 47
    Width = 257
    Height = 3
    Shape = bsTopLine
  end
  object editCount: TEdit
    Left = 176
    Top = 16
    Width = 81
    Height = 21
    TabOrder = 0
  end
  object Button1: TButton
    Left = 64
    Top = 56
    Width = 75
    Height = 25
    Caption = #1054#1050
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 144
    Top = 56
    Width = 75
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 2
  end
end
