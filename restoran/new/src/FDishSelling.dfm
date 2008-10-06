object frmDishSelling: TfrmDishSelling
  Left = 15
  Top = 173
  Width = 647
  Height = 375
  Caption = #1055#1088#1086#1076#1072#1085#1085#1099#1077' '#1073#1083#1102#1076#1072
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
    Left = 0
    Top = 170
    Width = 639
    Height = 4
    Cursor = crVSplit
    Align = alTop
    Beveled = True
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 639
    Height = 22
    Align = alTop
    Alignment = taLeftJustify
    BevelOuter = bvNone
    TabOrder = 0
    object DBNavigatorEx1: TDBNavigatorEx
      Left = 409
      Top = 0
      Width = 230
      Height = 22
      DataSource = dsDishSelling
      Align = alRight
      Flat = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
    object comboDish: TComboBox
      Left = 0
      Top = 0
      Width = 177
      Height = 21
      Hint = #1041#1083#1102#1076#1072
      Style = csDropDownList
      DropDownCount = 16
      ItemHeight = 13
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = comboDishChange
    end
    object comboMeal: TComboBox
      Left = 178
      Top = 0
      Width = 177
      Height = 21
      Hint = #1055#1088#1086#1076#1091#1082#1090#1099
      Style = csDropDownList
      DropDownCount = 16
      ItemHeight = 13
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnChange = comboMealChange
    end
  end
  object gridDishes: TDBGridEx
    Left = 0
    Top = 22
    Width = 639
    Height = 148
    Align = alTop
    DataSource = dsDishSelling
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'SelDate'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DishName'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Count'
        Title.Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' ('#1096#1090')'
        Visible = True
      end>
  end
  object Panel2: TPanel
    Left = 0
    Top = 174
    Width = 639
    Height = 22
    Align = alTop
    Alignment = taLeftJustify
    BevelOuter = bvNone
    Caption = #1050#1086#1084#1087#1086#1085#1077#1085#1090#1099':'
    TabOrder = 2
    object DBNavigatorEx2: TDBNavigatorEx
      Left = 409
      Top = 0
      Width = 230
      Height = 22
      DataSource = dsSoldDishComp
      Align = alRight
      Flat = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
  end
  object DBGridEx1: TDBGridEx
    Left = 0
    Top = 196
    Width = 639
    Height = 152
    Align = alClient
    DataSource = dsSoldDishComp
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
  object tblDishSelling: TADOTable
    Connection = dmodMain.connDB
    AfterInsert = tblDishSellingAfterInsert
    BeforePost = tblDishSellingBeforePost
    AfterPost = tblDishSellingAfterPost
    BeforeDelete = tblDishSellingBeforeDelete
    OnFilterRecord = tblDishSellingFilterRecord
    TableName = 'DishesSelling'
    Left = 108
    Top = 104
    object tblDishSellingId: TAutoIncField
      AutoGenerateValue = arAutoInc
      FieldName = 'Id'
      ReadOnly = True
      Visible = False
    end
    object tblDishSellingSelDate: TDateTimeField
      DisplayLabel = #1044#1072#1090#1072' '#1087#1088#1086#1076#1072#1078#1080
      FieldName = 'SelDate'
    end
    object tblDishSellingDishId: TIntegerField
      FieldName = 'DishId'
      Visible = False
    end
    object tblDishSellingDishName: TStringField
      DisplayLabel = #1041#1083#1102#1076#1086
      DisplayWidth = 50
      FieldKind = fkLookup
      FieldName = 'DishName'
      LookupDataSet = tblDishes
      LookupKeyFields = 'Id'
      LookupResultField = 'Name'
      KeyFields = 'DishId'
      Size = 255
      Lookup = True
    end
    object tblDishSellingCount: TBCDField
      FieldName = 'Count'
      Precision = 19
    end
  end
  object dsDishSelling: TDataSource
    DataSet = tblDishSelling
    Left = 204
    Top = 108
  end
  object tblDishes: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'Dishes'
    Left = 308
    Top = 104
    object tblDishesId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
    end
    object tblDishesName: TWideStringField
      FieldName = 'Name'
      Size = 255
    end
    object tblDishesTypeId: TIntegerField
      FieldName = 'TypeId'
    end
  end
  object tblSoldDishComp: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    BeforePost = tblSoldDishCompBeforePost
    AfterPost = tblSoldDishCompAfterPost
    BeforeDelete = tblSoldDishCompBeforeDelete
    IndexFieldNames = 'SoldDishId'
    MasterFields = 'Id'
    MasterSource = dsDishSelling
    TableName = 'SoldDishComponents'
    Left = 128
    Top = 228
    object tblSoldDishCompSoldDishId: TIntegerField
      FieldName = 'SoldDishId'
      Visible = False
    end
    object tblSoldDishCompMealId: TIntegerField
      FieldName = 'MealId'
      Visible = False
    end
    object tblSoldDishCompMealName: TStringField
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
    object tblSoldDishCompQuantity: TBCDField
      DisplayLabel = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' ('#1075#1088'.)'
      FieldName = 'Quantity'
      Precision = 19
    end
  end
  object dsSoldDishComp: TDataSource
    DataSet = tblSoldDishComp
    Left = 272
    Top = 228
  end
  object qry: TADOQuery
    Connection = dmodMain.connDB
    Parameters = <>
    Left = 424
    Top = 240
  end
  object tblMeal: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'Meal'
    Left = 360
    Top = 288
  end
  object tblSoldCompAll: TADOTable
    Connection = dmodMain.connDB
    IndexName = 'MealId'
    TableName = 'SoldDishComponents'
    Left = 216
    Top = 284
  end
end
