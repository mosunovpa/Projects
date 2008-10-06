object frmOperations: TfrmOperations
  Left = 21
  Top = 155
  Width = 684
  Height = 399
  Caption = #1055#1088#1080#1093#1086#1076'/'#1056#1072#1089#1093#1086#1076
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
    Width = 676
    Height = 22
    Align = alTop
    Alignment = taLeftJustify
    BevelOuter = bvNone
    TabOrder = 1
    object DBNavigatorEx1: TDBNavigatorEx
      Left = 446
      Top = 0
      Width = 230
      Height = 22
      DataSource = dsOperations
      Align = alRight
      Flat = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
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
    object comboOperations: TComboBox
      Left = 178
      Top = 0
      Width = 111
      Height = 21
      Hint = #1054#1087#1077#1088#1094#1080#1103' / '#1058#1080#1087
      Style = csDropDownList
      ItemHeight = 13
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnChange = comboOperationsChange
    end
  end
  object DBGrid: TDBGridEx
    Left = 0
    Top = 22
    Width = 676
    Height = 350
    Align = alClient
    DataSource = dsOperations
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    Columns = <
      item
        Expanded = False
        FieldName = 'OperationDate'
        Visible = True
      end
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
      end
      item
        Expanded = False
        FieldName = 'OperationName'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'SubType'
        Title.Caption = #1058#1080#1087
        Visible = True
      end>
  end
  object tblOperations: TADOTable
    Connection = dmodMain.connDB
    AfterInsert = tblOperationsAfterInsert
    BeforePost = tblOperationsBeforePost
    AfterPost = tblOperationsAfterPost
    TableName = 'MealOperations'
    Left = 124
    Top = 124
    object tblOperationsId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
      Visible = False
    end
    object tblOperationsOperationDate: TDateTimeField
      DisplayLabel = #1044#1072#1090#1072' '#1086#1087#1077#1088#1072#1094#1080#1080
      FieldName = 'OperationDate'
    end
    object tblOperationsMealId: TIntegerField
      FieldName = 'MealId'
      Visible = False
    end
    object tblOperationsMealName: TStringField
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
    object tblOperationsQuantity: TBCDField
      DisplayLabel = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' ('#1075#1088'.)'
      DisplayWidth = 15
      FieldName = 'Quantity'
      Precision = 19
    end
    object tblOperationsOperationId: TWordField
      FieldName = 'OperationId'
      Visible = False
    end
    object tblOperationsOperationName: TStringField
      DisplayLabel = #1054#1087#1077#1088#1072#1094#1080#1103
      DisplayWidth = 20
      FieldKind = fkLookup
      FieldName = 'OperationName'
      LookupDataSet = tblOperationTypes
      LookupKeyFields = 'Id'
      LookupResultField = 'Name'
      KeyFields = 'OperationId'
      Size = 255
      Lookup = True
    end
    object tblOperationsType: TIntegerField
      FieldName = 'Type'
    end
    object tblOperationsSubType: TStringField
      FieldKind = fkLookup
      FieldName = 'SubType'
      LookupDataSet = tblOperationSubTypes
      LookupKeyFields = 'id'
      LookupResultField = 'name'
      KeyFields = 'Type'
      Size = 50
      Lookup = True
    end
  end
  object tblMeal: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'Meal'
    Left = 228
    Top = 124
  end
  object tblOperationTypes: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    TableName = 'OperationTypes'
    Left = 332
    Top = 124
    object tblOperationTypesId: TWordField
      FieldName = 'Id'
    end
    object tblOperationTypesName: TWideStringField
      FieldName = 'Name'
      Size = 255
    end
  end
  object dsOperations: TDataSource
    DataSet = tblOperations
    Left = 124
    Top = 196
  end
  object tblOperationSubTypes: TADOTable
    Connection = dmodMain.connDB
    TableName = 'OperationSubTypes'
    Left = 340
    Top = 184
  end
end
