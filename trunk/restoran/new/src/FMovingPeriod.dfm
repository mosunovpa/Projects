object frmMovingPeriod: TfrmMovingPeriod
  Left = 221
  Top = 296
  BorderStyle = bsDialog
  Caption = #1055#1077#1088#1077#1085#1086#1089' '#1086#1089#1090#1072#1090#1082#1086#1074
  ClientHeight = 183
  ClientWidth = 285
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object btnOk: TButton
    Left = 122
    Top = 152
    Width = 75
    Height = 25
    Caption = #1054'&'#1050
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = btnOkClick
  end
  object Button2: TButton
    Left = 202
    Top = 152
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&'#1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 1
  end
  object Sheet: TPageControl
    Left = 8
    Top = 8
    Width = 269
    Height = 137
    ActivePage = PageMonth
    TabIndex = 0
    TabOrder = 2
    object PageMonth: TTabSheet
      Caption = #1052#1077#1089#1103#1094
      object Label4: TLabel
        Left = 8
        Top = 12
        Width = 174
        Height = 26
        Caption = #1059#1082#1072#1078#1080#1090#1077' '#1084#1077#1089#1103#1094' '#1085#1072' '#1082#1086#1090#1086#1088#1099#1081' '#1073#1091#1076#1091#1090' '#1087#1077#1088#1077#1085#1077#1089#1077#1085#1099' '#1086#1089#1090#1072#1090#1082#1080':'
        WordWrap = True
      end
      object Label5: TLabel
        Left = 8
        Top = 52
        Width = 36
        Height = 13
        Caption = #1052#1077#1089#1103#1094':'
      end
      object comboMonth: TMonthYearComboBox
        Left = 140
        Top = 48
        Width = 109
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object PagePeriod: TTabSheet
      Caption = #1055#1077#1088#1080#1086#1076
      ImageIndex = 1
      object Label1: TLabel
        Left = 8
        Top = 12
        Width = 210
        Height = 26
        Caption = 
          #1059#1082#1072#1078#1080#1090#1077' '#1085#1072#1095#1072#1083#1100#1085#1099#1081' '#1080' '#1082#1086#1085#1077#1095#1085#1099#1081' '#1084#1077#1089#1103#1094#1099' '#1085#1072' '#1082#1086#1090#1086#1088#1099#1077' '#1073#1091#1076#1091#1090' '#1087#1077#1088#1077#1085#1077#1089#1077#1085#1099' ' +
          #1086#1089#1090#1072#1090#1082#1080':'
        WordWrap = True
      end
      object Label2: TLabel
        Left = 8
        Top = 52
        Width = 95
        Height = 13
        Caption = #1053#1072#1095#1072#1083#1100#1085#1099#1081' '#1084#1077#1089#1103#1094':'
      end
      object Label3: TLabel
        Left = 8
        Top = 80
        Width = 88
        Height = 13
        Caption = #1050#1086#1085#1077#1095#1085#1099#1081' '#1084#1077#1089#1103#1094':'
      end
      object comboBeginMonth: TMonthYearComboBox
        Left = 140
        Top = 48
        Width = 109
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
      end
      object comboEndMonth: TMonthYearComboBox
        Left = 140
        Top = 76
        Width = 109
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 1
      end
    end
  end
end
