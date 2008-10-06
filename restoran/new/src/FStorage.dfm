object frmStorage: TfrmStorage
  Left = 35
  Top = 157
  Width = 544
  Height = 390
  Caption = #1054#1089#1090#1072#1090#1082#1080
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
  object pnlStorage: TPanel
    Left = 0
    Top = 0
    Width = 536
    Height = 22
    Align = alTop
    Alignment = taLeftJustify
    BevelOuter = bvNone
    TabOrder = 0
    object DBNavigatorEx1: TDBNavigatorEx
      Left = 421
      Top = 0
      Width = 115
      Height = 22
      DataSource = dsStorage
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbRefresh]
      Align = alRight
      Flat = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      BeforeAction = DBNavigatorEx1BeforeAction
    end
    object comboMonth: TMonthYearComboBox
      Left = 178
      Top = 0
      Width = 105
      Height = 21
      Hint = #1052#1077#1089#1103#1094' '#1086#1089#1090#1072#1090#1082#1086#1074
      Style = csDropDownList
      ItemHeight = 13
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = comboMonthChange
    end
    object comboMeal: TComboBox
      Left = 0
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
  object DBGrid1: TDBGridEx
    Left = 0
    Top = 22
    Width = 536
    Height = 341
    Align = alClient
    DataSource = dsStorage
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
    ReadOnly = True
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'Date'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Name'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Quantity'
        Visible = True
      end>
  end
  object qryStorage: TADOQuery
    Connection = dmodMain.connDB
    Parameters = <
      item
        Name = 'p'
        Attributes = [paNullable]
        DataType = ftFixedChar
        NumericScale = 255
        Precision = 255
        Size = 510
        Value = Null
      end>
    SQL.Strings = (
      
        'SELECT Meal.Id, MealStorage.Date, Meal.Name, MealStorage.Quantit' +
        'y'
      'FROM Meal INNER JOIN MealStorage ON Meal.Id = MealStorage.MealId'
      'WHERE MealStorage.Date = :p'
      'ORDER BY Meal.Name')
    Left = 124
    Top = 108
    object qryStorageDate: TDateTimeField
      DisplayLabel = #1044#1072#1090#1072
      FieldName = 'Date'
    end
    object qryStorageName: TWideStringField
      DisplayLabel = #1055#1088#1086#1076#1091#1082#1090
      DisplayWidth = 50
      FieldName = 'Name'
      Size = 255
    end
    object qryStorageQuantity: TBCDField
      DisplayLabel = #1054#1089#1090#1072#1090#1086#1082' ('#1075#1088'.)'
      DisplayWidth = 15
      FieldName = 'Quantity'
      Precision = 19
    end
  end
  object dsStorage: TDataSource
    DataSet = qryStorage
    Left = 192
    Top = 112
  end
  object tblMeal: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'Meal'
    Left = 148
    Top = 232
  end
end
