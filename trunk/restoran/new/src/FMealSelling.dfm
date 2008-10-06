object frmMealSelling: TfrmMealSelling
  Left = 47
  Top = 188
  Width = 544
  Height = 375
  Caption = #1055#1088#1086#1076#1072#1085#1085#1099#1077' '#1087#1088#1086#1076#1091#1082#1090#1099
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
    TabOrder = 1
    object DBNavigatorEx1: TDBNavigatorEx
      Left = 421
      Top = 0
      Width = 115
      Height = 22
      DataSource = dsMealSelling
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbRefresh]
      Align = alRight
      Flat = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
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
      TabOrder = 1
      OnChange = comboMealChange
    end
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 22
    Width = 536
    Height = 326
    Align = alClient
    DataSource = dsMealSelling
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
    ReadOnly = True
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'SelDate'
        Title.Caption = #1044#1072#1090#1072' '#1087#1088#1086#1076#1072#1078#1080
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Meal.Name'
        Title.Caption = #1055#1088#1086#1076#1091#1082#1090
        Width = 150
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Id'
        Title.Caption = #1053#1086#1084#1077#1088' '#1073#1083#1102#1076#1072
        Visible = False
      end
      item
        Expanded = False
        FieldName = 'Dishes.Name'
        Title.Caption = #1041#1083#1102#1076#1086
        Width = 150
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Quantity'
        Title.Caption = #1042#1077#1089' ('#1075#1088'.)'
        Width = 90
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Count'
        Title.Caption = #1050#1086#1083'. '#1073#1083#1102#1076' ('#1096#1090'.)'
        Visible = True
      end>
  end
  object dsMealSelling: TDataSource
    DataSet = qryMealSelling
    Left = 328
    Top = 80
  end
  object qryMealSelling: TADOQuery
    Connection = dmodMain.connDB
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      'SELECT DishesSelling.SelDate, Meal.Id as MealId, Meal.Name,  '
      '  DishesSelling.Id, Dishes.Name, SoldDishComponents.Quantity,'
      '  DishesSelling.Count'
      'FROM Meal INNER JOIN '
      '('
      '  ('
      
        '  Dishes INNER JOIN DishesSelling ON Dishes.Id = DishesSelling.D' +
        'ishId'
      
        '  ) INNER JOIN SoldDishComponents ON DishesSelling.Id = SoldDish' +
        'Components.SoldDishId'
      ') ON Meal.Id = SoldDishComponents.MealId'
      'order by DishesSelling.SelDate desc, Meal.Name, Dishes.Name')
    Left = 120
    Top = 112
  end
  object tblMeal: TADOTable
    Connection = dmodMain.connDB
    IndexFieldNames = 'Name'
    TableName = 'Meal'
    Left = 260
    Top = 172
  end
end
