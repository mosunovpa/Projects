object frmMovingEx: TfrmMovingEx
  Left = 20
  Top = 161
  Width = 553
  Height = 375
  Caption = #1054#1073#1086#1088#1086#1090#1099
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  KeyPreview = True
  Menu = MovingMenu
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Grid: TDBGridEx
    Left = 0
    Top = 22
    Width = 545
    Height = 307
    Align = alClient
    DataSource = dsMoving
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit]
    ReadOnly = True
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDblClick = acChangeExecute
    Columns = <
      item
        Expanded = False
        FieldName = 'Name'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'StartQuantity'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ReceiptEnd'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'ChargeEnd'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DishChargeEnd'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Quantity'
        Visible = True
      end>
  end
  object pnlStorage: TPanel
    Left = 0
    Top = 0
    Width = 545
    Height = 22
    Align = alTop
    Alignment = taLeftJustify
    BevelOuter = bvNone
    TabOrder = 1
    object DBNavigatorEx1: TDBNavigatorEx
      Left = 430
      Top = 0
      Width = 115
      Height = 22
      DataSource = dsMoving
      VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbRefresh]
      Align = alRight
      Flat = True
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
    end
    object calendarDateFrom: TDateTimePicker
      Left = 178
      Top = 0
      Width = 88
      Height = 21
      Hint = #1053#1072#1095#1072#1083#1086' '#1087#1077#1088#1080#1086#1076#1072
      CalAlignment = dtaLeft
      Date = 38382.9623172569
      Time = 38382.9623172569
      DateFormat = dfShort
      DateMode = dmComboBox
      Kind = dtkDate
      ParseInput = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
    end
    object calendarDateTo: TDateTimePicker
      Left = 266
      Top = 0
      Width = 88
      Height = 21
      Hint = #1050#1086#1085#1077#1094' '#1087#1077#1088#1080#1086#1076#1072
      CalAlignment = dtaLeft
      Date = 38382.9623172569
      Time = 38382.9623172569
      DateFormat = dfShort
      DateMode = dmComboBox
      Kind = dtkDate
      ParseInput = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
    end
    object ToolBar1: TToolBar
      Left = 357
      Top = 0
      Width = 54
      Height = 22
      Align = alNone
      Caption = 'ToolBar1'
      EdgeBorders = []
      Flat = True
      Images = Images
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      object ToolButton1: TToolButton
        Left = 0
        Top = 0
        Action = acRefresh
      end
      object ToolButton2: TToolButton
        Left = 23
        Top = 0
        Width = 8
        Caption = 'ToolButton2'
        ImageIndex = 1
        Style = tbsSeparator
      end
      object ToolButton3: TToolButton
        Left = 31
        Top = 0
        Action = acChange
      end
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
      TabOrder = 4
      OnChange = comboMealChange
    end
  end
  object qryMoving: TADOQuery
    Connection = dmodMain.connDB
    OnCalcFields = qryMovingCalcFields
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
      'SELECT MealStorage.Date, Meal.Name, MealStorage.Quantity'
      'FROM Meal INNER JOIN MealStorage ON Meal.Id = MealStorage.MealId'
      'WHERE MealStorage.Date = :p'
      'ORDER BY Meal.Name')
    Left = 124
    Top = 108
    object qryMovingId: TIntegerField
      FieldName = 'Id'
    end
    object qryMovingName: TStringField
      DisplayLabel = #1055#1088#1086#1076#1091#1082#1090
      DisplayWidth = 50
      FieldName = 'Name'
      Size = 255
    end
    object qryMovingStorage: TBCDField
      DisplayLabel = #1053#1072' '#1085#1072#1095#1072#1083#1086' '#1084#1077#1089#1103#1094#1072
      FieldName = 'Storage'
      Visible = False
      Size = 20
    end
    object qryMovingReceipt: TBCDField
      DisplayLabel = #1055#1088#1080#1093#1086#1076' ('#1075#1088'.)'
      FieldName = 'Receipt'
      Visible = False
      Size = 20
    end
    object qryMovingCharge: TBCDField
      DisplayLabel = #1056#1072#1089#1093#1086#1076' ('#1075#1088'.)'
      FieldName = 'Charge'
      Visible = False
      Size = 20
    end
    object qryMovingDishCharge: TBCDField
      DisplayLabel = #1055#1086' '#1073#1083#1102#1076#1072#1084' ('#1075#1088'.)'
      FieldName = 'DishCharge'
      Visible = False
      Size = 20
    end
    object qryMovingStartQuantity: TBCDField
      DisplayLabel = #1054#1089#1090#1072#1090#1086#1082' '#1085#1072' '#1085#1072#1095#1072#1083#1086' ('#1075#1088'.)'
      FieldKind = fkCalculated
      FieldName = 'StartQuantity'
      Size = 20
      Calculated = True
    end
    object qryMovingReceiptEnd: TBCDField
      DisplayLabel = #1055#1088#1080#1093#1086#1076' ('#1075#1088'.)'
      FieldName = 'ReceiptEnd'
      Size = 20
    end
    object qryMovingChargeEnd: TBCDField
      DisplayLabel = #1056#1072#1089#1093#1086#1076' ('#1075#1088'.)'
      FieldName = 'ChargeEnd'
      Size = 20
    end
    object qryMovingDishChargeEnd: TBCDField
      DisplayLabel = #1055#1086' '#1073#1083#1102#1076#1072#1084' ('#1075#1088'.)'
      FieldName = 'DishChargeEnd'
      Size = 20
    end
    object qryMovingQuantity: TBCDField
      DisplayLabel = #1054#1089#1090#1072#1090#1086#1082' ('#1075#1088'.)'
      FieldKind = fkCalculated
      FieldName = 'Quantity'
      Size = 20
      Calculated = True
    end
  end
  object dsMoving: TDataSource
    DataSet = qryMoving
    Left = 192
    Top = 112
  end
  object Images: TImageList
    Left = 288
    Top = 184
    Bitmap = {
      494C010102000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001001000000000000008
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000EF3DEF3D0000EF3D
      EF3DEF3DEF3DEF3DEF3D0000EF3DEF3D00000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000007C007C
      007C007C007C0000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000EF3D0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000007C007C007C007C
      007C007C007C007C007C00000000000000000000000000000000000000000000
      00000000000000000000F75EEF3D0000EF3D0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000007C007C007C00000000
      000000000000007C007C007C0000000000000000000000000000000000000000
      0000000000000000F75EF75EF75E000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000007C007C007C000000000000
      0000000000000000007C007C007C000000000000000000000000000000000000
      00000000000000420042F75E0000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000007C007C0000000000000000
      00000000000000000000007C007C000000000000000000000000000000000000
      0000000000420042E07F00000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000007C007C00000000000000000000
      000000000000000000000000007C007C00000000000000000000000000000000
      000000420042E07F000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000007C007C00000000000000000000
      000000000000000000000000007C007C00000000000000000000000000000000
      00420042E07F0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000007C007C00000000000000000000
      0000000000000000000000000000000000000000000000000000000000000042
      0042E07F00000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000007C007C00000000000000000000
      0000000000000000000000000000000000000000000000000000000000420042
      E07F000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000007C007C00000000000000000000
      000000000000007C007C007C007C007C0000000000000000000000420042E07F
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000007C007C0000000000000000
      0000000000000000007C007C007C007C000000000000000000420000E07F0000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000007C007C007C000000000000
      00000000000000000000007C007C007C00000000000000000000E07F00000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000007C007C007C00000000
      000000000000007C007C007C007C007C000000000040007C0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000007C007C007C007C
      007C007C007C007C007C00000000007C00000000007C007C0000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000007C007C
      007C007C007C0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFFE40800000000F83FFFF000000000
      E00FFFE000000000C7C7FFC1000000008FE3FF83000000009FF3FF0700000000
      3FF9FE0F000000003FF9FC1F000000003FFFF83F000000003FFFF07F00000000
      3FC1E0FF000000009FE1C1FF000000008FF183FF00000000C7C107FF00000000
      E00D0FFF00000000F83F9FFF0000000000000000000000000000000000000000
      000000000000}
  end
  object MovingActions: TActionList
    Images = Images
    Left = 288
    Top = 112
    object acRefresh: TAction
      Caption = '&'#1054#1073#1085#1086#1074#1080#1090#1100
      Hint = #1055#1077#1088#1077#1089#1095#1080#1090#1072#1090#1100' '#1086#1073#1086#1088#1086#1090#1099
      ImageIndex = 0
      OnExecute = btnShowClick
    end
    object acChange: TAction
      Caption = '&'#1050#1086#1088#1088#1077#1082#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1089#1090#1072#1090#1082#1080'...'
      Hint = #1050#1086#1088#1088#1077#1082#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1089#1090#1072#1090#1082#1080
      ImageIndex = 1
      OnExecute = acChangeExecute
    end
  end
  object MovingMenu: TMainMenu
    Images = Images
    Left = 376
    Top = 164
    object N1: TMenuItem
      Caption = #1054'&'#1073#1086#1088#1086#1090#1099
      GroupIndex = 3
      object N2: TMenuItem
        Action = acRefresh
      end
      object N3: TMenuItem
        Action = acChange
      end
    end
  end
  object tblMeal: TADOTable
    Connection = dmodMain.connDB
    CursorType = ctStatic
    IndexFieldNames = 'Name'
    TableName = 'Meal'
    Left = 148
    Top = 228
  end
end
