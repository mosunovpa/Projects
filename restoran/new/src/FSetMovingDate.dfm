object frmSetMovingDate: TfrmSetMovingDate
  Left = 209
  Top = 196
  BorderStyle = bsDialog
  Caption = #1054#1073#1086#1088#1086#1090#1099
  ClientHeight = 179
  ClientWidth = 277
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
  object Sheet: TPageControl
    Left = 8
    Top = 8
    Width = 261
    Height = 133
    ActivePage = PagePeriod
    TabIndex = 1
    TabOrder = 2
    object PageDate: TTabSheet
      Caption = #1057' '#1085#1072#1095#1072#1083#1072' '#1084#1077#1089#1103#1094#1072
      object Label1: TLabel
        Left = 8
        Top = 8
        Width = 232
        Height = 26
        Caption = 
          #1059#1082#1072#1078#1080#1090#1077' '#1076#1072#1090#1091', '#1085#1072' '#1082#1086#1090#1086#1088#1091#1102' '#1074#1099' '#1093#1086#1090#1080#1090#1077' '#1074#1099#1074#1077#1089#1090#1080' '#1086#1073#1086#1088#1086#1090#1099' '#1089' '#1085#1072#1095#1072#1083#1072' '#1084#1077#1089#1103 +
          #1094#1072':'
        WordWrap = True
      end
      object calendarDate: TDateTimePicker
        Left = 8
        Top = 44
        Width = 129
        Height = 21
        CalAlignment = dtaLeft
        Date = 38386.9509715278
        Time = 38386.9509715278
        DateFormat = dfLong
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 0
      end
    end
    object PagePeriod: TTabSheet
      Caption = #1047#1072' '#1087#1077#1088#1080#1086#1076
      ImageIndex = 1
      object Label2: TLabel
        Left = 8
        Top = 8
        Width = 204
        Height = 26
        Caption = #1059#1082#1072#1078#1080#1090#1077' '#1087#1077#1088#1080#1086#1076', '#1079#1072' '#1082#1086#1090#1086#1088#1099#1081' '#1074#1099' '#1093#1086#1090#1080#1090#1077' '#1074#1099#1074#1077#1089#1090#1080' '#1086#1073#1086#1088#1086#1090#1099':'
        WordWrap = True
      end
      object Label3: TLabel
        Left = 8
        Top = 48
        Width = 79
        Height = 13
        Caption = #1053#1072#1095#1072#1083' '#1087#1077#1088#1080#1086#1076#1072':'
      end
      object Label4: TLabel
        Left = 8
        Top = 76
        Width = 76
        Height = 13
        Caption = #1050#1086#1085#1077#1094' '#1087#1077#1088#1080#1086#1076#1072
      end
      object calendarFrom: TDateTimePicker
        Left = 112
        Top = 44
        Width = 129
        Height = 21
        CalAlignment = dtaLeft
        Date = 38386.9509715278
        Time = 38386.9509715278
        DateFormat = dfLong
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 0
      end
      object calendarTo: TDateTimePicker
        Left = 112
        Top = 72
        Width = 129
        Height = 21
        CalAlignment = dtaLeft
        Date = 38386.9509715278
        Time = 38386.9509715278
        DateFormat = dfLong
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 1
      end
    end
  end
  object Button1: TButton
    Left = 114
    Top = 148
    Width = 75
    Height = 25
    Caption = #1054'&'#1050
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 194
    Top = 148
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&'#1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 1
  end
end
