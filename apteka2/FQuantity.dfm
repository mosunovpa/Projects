object frmQuantity: TfrmQuantity
  Left = 433
  Top = 215
  BorderStyle = bsDialog
  Caption = #1040#1087#1090#1077#1082#1072
  ClientHeight = 191
  ClientWidth = 303
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 6
    Top = 104
    Width = 162
    Height = 13
    Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1091#1087#1072#1082#1086#1074#1086#1082' '#1085#1072' '#1084#1077#1089#1103#1094':'
  end
  object Bevel1: TBevel
    Left = 8
    Top = 152
    Width = 289
    Height = 3
    Shape = bsTopLine
  end
  object Label2: TLabel
    Left = 96
    Top = 127
    Width = 71
    Height = 13
    Caption = #1054#1073#1086#1089#1085#1086#1074#1072#1085#1080#1077':'
  end
  object Label3: TLabel
    Left = 17
    Top = 80
    Width = 148
    Height = 13
    Caption = #1056#1072#1079'.'#1076#1086#1079#1072', '#1082#1088#1072#1090#1085#1086#1089#1090#1100' '#1087#1088#1080#1077#1084#1072':'
  end
  object Label4: TLabel
    Left = 8
    Top = 8
    Width = 46
    Height = 13
    Caption = #1055#1072#1094#1080#1077#1085#1090':'
  end
  object lblPatient: TLabel
    Left = 72
    Top = 8
    Width = 43
    Height = 13
    Caption = 'lblPatient'
  end
  object Label6: TLabel
    Left = 8
    Top = 30
    Width = 52
    Height = 13
    Caption = #1055#1088#1077#1087#1072#1088#1072#1090':'
  end
  object lblMedicine: TLabel
    Left = 72
    Top = 30
    Width = 225
    Height = 25
    AutoSize = False
    Caption = 'lblMedicine'
    WordWrap = True
  end
  object Bevel2: TBevel
    Left = 8
    Top = 64
    Width = 289
    Height = 3
    Shape = bsTopLine
  end
  object Edit1: TEdit
    Left = 176
    Top = 99
    Width = 41
    Height = 21
    TabOrder = 1
    Text = '1'
  end
  object updwQuantity: TUpDown
    Left = 217
    Top = 99
    Width = 15
    Height = 21
    Associate = Edit1
    Min = 1
    Position = 1
    TabOrder = 2
  end
  object Button1: TButton
    Left = 80
    Top = 160
    Width = 75
    Height = 25
    Caption = #1054#1050
    Default = True
    ModalResult = 1
    TabOrder = 4
  end
  object Button2: TButton
    Left = 160
    Top = 160
    Width = 75
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 5
  end
  object edDescription: TEdit
    Left = 176
    Top = 123
    Width = 121
    Height = 21
    TabOrder = 3
  end
  object edDosa: TEdit
    Left = 176
    Top = 75
    Width = 121
    Height = 21
    TabOrder = 0
  end
end
