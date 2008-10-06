object frmDishes: TfrmDishes
  Left = 26
  Top = 119
  Width = 597
  Height = 410
  Caption = #1041#1083#1102#1076#1072
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
  object spltTypes: TSplitter
    Left = 185
    Top = 0
    Width = 4
    Height = 383
    Cursor = crHSplit
  end
  object Panel2: TPanel
    Left = 189
    Top = 0
    Width = 400
    Height = 383
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object spltDishes: TSplitter
      Left = 0
      Top = 214
      Width = 400
      Height = 4
      Cursor = crVSplit
      Align = alTop
      Beveled = True
    end
    object gridDishes: TDBGridEx
      Left = 0
      Top = 22
      Width = 400
      Height = 192
      Align = alTop
      DataSource = dsDishes
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
    object pnlComponents: TPanel
      Left = 0
      Top = 218
      Width = 400
      Height = 22
      Align = alTop
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = ' '#1050#1086#1084#1087#1086#1085#1077#1085#1090#1099':'
      TabOrder = 1
      object DBNavigatorEx2: TDBNavigatorEx
        Left = 170
        Top = 0
        Width = 230
        Height = 22
        DataSource = dsDishComponents
        Align = alRight
        Flat = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        BeforeAction = DBNavigatorEx2BeforeAction
      end
    end
    object pnlDishes: TPanel
      Left = 0
      Top = 0
      Width = 400
      Height = 22
      Align = alTop
      Alignment = taLeftJustify
      BevelOuter = bvNone
      Caption = #1041#1083#1102#1076#1072':'
      TabOrder = 2
      object DBNavigatorEx1: TDBNavigatorEx
        Left = 170
        Top = 0
        Width = 230
        Height = 22
        DataSource = dsDishes
        Align = alRight
        Flat = True
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        BeforeAction = DBNavigatorEx1BeforeAction
      end
    end
    object DBGrid2: TDBGridEx
      Left = 0
      Top = 240
      Width = 400
      Height = 143
      Align = alClient
      DataSource = dsDishComponents
      TabOrder = 3
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
      Columns = <
        item
          Expanded = False
          FieldName = 'MealName'
          Visible = True
        end
        item
          Expanded = False
          FieldName = 'Quantity'
          Title.Caption = #1042#1077#1089' ('#1075#1088'.)'
          Visible = True
        end>
    end
  end
  object pnlDishCategories: TPanel
    Left = 0
    Top = 0
    Width = 185
    Height = 383
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    object Panel5: TPanel
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
    object tvTypes: TTreeView
      Left = 0
      Top = 22
      Width = 185
      Height = 361
      Align = alClient
      HideSelection = False
      Indent = 19
      ReadOnly = True
      TabOrder = 0
      OnChange = tvTypesChange
    end
  end
  object tblTypes: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'DishesTypes'
    Left = 52
    Top = 88
    object tblTypesId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
    end
    object tblTypesName: TWideStringField
      FieldName = 'Name'
      Size = 255
    end
  end
  object tblDishes: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    AfterInsert = tblDishesAfterInsert
    BeforePost = tblDishesBeforePost
    OnDeleteError = tblDishesDeleteError
    TableName = 'Dishes'
    Left = 264
    Top = 96
    object tblDishesId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
      Visible = False
    end
    object tblDishesName: TWideStringField
      DisplayLabel = #1041#1083#1102#1076#1086
      DisplayWidth = 50
      FieldName = 'Name'
      Size = 255
    end
    object tblDishesTypeId: TIntegerField
      FieldName = 'TypeId'
      Visible = False
    end
    object tblDishesTypeName: TStringField
      DisplayLabel = #1050#1072#1090#1077#1075#1086#1088#1080#1103
      DisplayWidth = 20
      FieldKind = fkLookup
      FieldName = 'TypeName'
      LookupDataSet = tblTypes
      LookupKeyFields = 'Id'
      LookupResultField = 'Name'
      KeyFields = 'TypeId'
      Size = 100
      Lookup = True
    end
  end
  object dsDishes: TDataSource
    DataSet = tblDishes
    OnDataChange = dsDishesDataChange
    Left = 320
    Top = 96
  end
  object tblDishComponents: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    BeforePost = tblDishComponentsBeforePost
    IndexFieldNames = 'DishId'
    MasterFields = 'Id'
    MasterSource = dsDishes
    TableName = 'DishComponents'
    Left = 268
    Top = 304
    object tblDishComponentsDishId: TIntegerField
      FieldName = 'DishId'
      Visible = False
    end
    object tblDishComponentsMealId: TIntegerField
      FieldName = 'MealId'
      Visible = False
    end
    object tblDishComponentsMealName: TStringField
      DisplayLabel = #1055#1088#1086#1076#1091#1082#1090
      DisplayWidth = 50
      FieldKind = fkLookup
      FieldName = 'MealName'
      LookupDataSet = tblMeal
      LookupKeyFields = 'Id'
      LookupResultField = 'Name'
      KeyFields = 'MealId'
      Size = 255
      Lookup = True
    end
    object tblDishComponentsQuantity: TBCDField
      DisplayLabel = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' ('#1075#1088'.)'
      FieldName = 'Quantity'
      Precision = 19
    end
  end
  object dsDishComponents: TDataSource
    DataSet = tblDishComponents
    Left = 368
    Top = 304
  end
  object tblMeal: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'Meal'
    Left = 440
    Top = 304
  end
end
