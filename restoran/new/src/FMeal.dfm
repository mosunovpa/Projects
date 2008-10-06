object frmMeal: TfrmMeal
  Left = 20
  Top = 206
  Width = 537
  Height = 312
  Caption = #1055#1088#1086#1076#1091#1082#1090#1099
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
  object Splitter1: TSplitter
    Left = 185
    Top = 0
    Width = 4
    Height = 285
    Cursor = crHSplit
  end
  object Panel2: TPanel
    Left = 189
    Top = 0
    Width = 340
    Height = 285
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object DBGrid1: TDBGridEx
      Left = 0
      Top = 22
      Width = 340
      Height = 263
      Align = alClient
      DataSource = dsMeal
      TabOrder = 0
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
        end
        item
          Expanded = False
          FieldName = 'TypeName'
          Visible = True
        end>
    end
    object pnlMeal: TPanel
      Left = 0
      Top = 0
      Width = 340
      Height = 22
      Align = alTop
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = ' '#1055#1088#1086#1076#1091#1082#1090#1099':'
      TabOrder = 1
      object DBNavigatorEx1: TDBNavigatorEx
        Left = 110
        Top = 0
        Width = 230
        Height = 22
        DataSource = dsMeal
        Align = alRight
        Flat = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        BeforeAction = DBNavigatorEx1BeforeAction
      end
    end
  end
  object pnlCategory: TPanel
    Left = 0
    Top = 0
    Width = 185
    Height = 285
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    object tvTypes: TTreeView
      Left = 0
      Top = 22
      Width = 185
      Height = 263
      Align = alClient
      HideSelection = False
      Indent = 19
      ReadOnly = True
      TabOrder = 0
      OnChange = tvTypesChange
    end
    object Panel4: TPanel
      Left = 0
      Top = 0
      Width = 185
      Height = 22
      Align = alTop
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = ' '#1050#1072#1090#1077#1075#1086#1088#1080#1080':'
      TabOrder = 1
    end
  end
  object tblTypes: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'MealTypes'
    Left = 32
    Top = 120
    object tblTypesId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
    end
    object tblTypesName: TWideStringField
      FieldName = 'Name'
      Size = 255
    end
  end
  object tblMeal: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    AfterInsert = tblMealAfterInsert
    BeforePost = tblMealBeforePost
    OnDeleteError = tblMealDeleteError
    TableName = 'Meal'
    Left = 264
    Top = 104
    object tblMealId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
      Visible = False
    end
    object tblMealName: TWideStringField
      DisplayLabel = #1055#1088#1086#1076#1091#1082#1090
      DisplayWidth = 50
      FieldName = 'Name'
      Size = 255
    end
    object tblMealTypeId: TIntegerField
      FieldName = 'TypeId'
      Visible = False
    end
    object tblMealTypeName: TStringField
      DisplayLabel = #1050#1072#1090#1077#1075#1086#1088#1080#1103
      DisplayWidth = 20
      FieldKind = fkLookup
      FieldName = 'TypeName'
      LookupDataSet = tblTypes
      LookupKeyFields = 'Id'
      LookupResultField = 'Name'
      KeyFields = 'TypeId'
      Size = 255
      Lookup = True
    end
  end
  object dsMeal: TDataSource
    DataSet = tblMeal
    Left = 320
    Top = 104
  end
end
