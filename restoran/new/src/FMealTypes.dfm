object frmMealTypes: TfrmMealTypes
  Left = 43
  Top = 136
  Width = 544
  Height = 375
  Caption = #1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1087#1088#1086#1076#1091#1082#1090#1086#1074
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 536
    Height = 22
    Align = alTop
    Alignment = taLeftJustify
    BevelOuter = bvNone
    Caption = ' '#1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1087#1088#1086#1076#1091#1082#1090#1086#1074':'
    TabOrder = 0
    object navMealTypes: TDBNavigatorEx
      Left = 306
      Top = 0
      Width = 230
      Height = 22
      DataSource = dsMealTypes
      Align = alRight
      Flat = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
  end
  object DBGrid1: TDBGridEx
    Left = 0
    Top = 22
    Width = 536
    Height = 326
    Align = alClient
    DataSource = dsMealTypes
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'Name'
        Visible = True
      end>
  end
  object tblMealTypes: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    BeforePost = tblMealTypesBeforePost
    OnDeleteError = tblMealTypesDeleteError
    TableName = 'MealTypes'
    Left = 76
    Top = 132
    object tblMealTypesId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
      Visible = False
    end
    object tblMealTypesName: TWideStringField
      DisplayLabel = #1050#1072#1090#1077#1075#1086#1088#1080#1103
      DisplayWidth = 50
      FieldName = 'Name'
      Size = 255
    end
  end
  object dsMealTypes: TDataSource
    DataSet = tblMealTypes
    Left = 156
    Top = 132
  end
end
