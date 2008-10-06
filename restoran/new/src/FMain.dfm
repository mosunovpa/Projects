object frmMain: TfrmMain
  Left = 62
  Top = 108
  Width = 430
  Height = 377
  Caption = #1056#1077#1089#1090#1086#1088#1072#1085
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = mnuMain
  OldCreateOrder = False
  Position = poDefault
  WindowMenu = Window1
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar: TStatusBar
    Left = 0
    Top = 312
    Width = 422
    Height = 19
    AutoHint = True
    Panels = <
      item
        Width = 50
      end>
    SimplePanel = True
  end
  object CoolBar: TCoolBar
    Left = 0
    Top = 0
    Width = 422
    Height = 49
    AutoSize = True
    Bands = <
      item
        Break = False
        Control = ToolBar
        ImageIndex = -1
        MinHeight = 22
        Width = 418
      end
      item
        Control = tabWindows
        ImageIndex = -1
        MinHeight = 21
        Width = 418
      end>
    object tabWindows: TTabControl
      Left = 9
      Top = 24
      Width = 405
      Height = 21
      Align = alTop
      Style = tsFlatButtons
      TabOrder = 0
      OnChange = tabWindowsChange
    end
    object ToolBar: TToolBar
      Left = 9
      Top = 0
      Width = 405
      Height = 22
      AutoSize = True
      Caption = 'ToolBar'
      EdgeBorders = []
      Flat = True
      Images = ImageList1
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      object ToolButton1: TToolButton
        Left = 0
        Top = 0
        Action = acDishesSelling
        DropdownMenu = menuSelling
        Style = tbsDropDown
      end
      object ToolButton2: TToolButton
        Left = 36
        Top = 0
        Action = acMealOperation
      end
      object ToolButton3: TToolButton
        Left = 59
        Top = 0
        Width = 8
        Caption = 'ToolButton3'
        ImageIndex = 2
        Style = tbsSeparator
      end
      object ToolButton4: TToolButton
        Left = 67
        Top = 0
        Action = acMoving
      end
    end
  end
  object mnuMain: TMainMenu
    Images = ImageList1
    Left = 44
    Top = 80
    object N1: TMenuItem
      Caption = '&'#1060#1072#1081#1083
      object N7: TMenuItem
        Caption = '-'
      end
      object N2: TMenuItem
        Action = acClose
      end
    end
    object N3: TMenuItem
      Caption = '&'#1054#1087#1077#1088#1072#1094#1080#1080
      GroupIndex = 1
      object N15: TMenuItem
        Action = asSetDate
      end
      object N16: TMenuItem
        Caption = '-'
      end
      object Meal1: TMenuItem
        Action = acMealOperation
      end
      object N22: TMenuItem
        Caption = '&'#1055#1088#1086#1076#1072#1085#1085#1099#1077
        Hint = #1055#1088#1086#1076#1072#1085#1085#1099#1077' '#1073#1083#1102#1076#1072' '#1080' '#1087#1088#1086#1076#1091#1082#1090#1099
        object N4: TMenuItem
          Action = acDishesSelling
        end
        object N23: TMenuItem
          Action = acMealSelling
        end
      end
      object N18: TMenuItem
        Caption = '-'
      end
      object N14: TMenuItem
        Action = acMoving
      end
      object N6: TMenuItem
        Action = acStorage
      end
      object N5: TMenuItem
        Action = acMoveRests
      end
    end
    object N8: TMenuItem
      Caption = '&'#1057#1083#1086#1074#1072#1088#1080
      GroupIndex = 5
      object N9: TMenuItem
        Action = acMealTypes
      end
      object acMeal1: TMenuItem
        Action = acMeal
      end
      object N19: TMenuItem
        Caption = '-'
      end
      object N10: TMenuItem
        Action = acDishTypes
      end
      object acDishes1: TMenuItem
        Action = acDishes
      end
    end
    object N21: TMenuItem
      Caption = #1057#1077'&'#1088#1074#1080#1089
      GroupIndex = 6
      object acDeleteData1: TMenuItem
        Action = acDeleteData
      end
    end
    object N17: TMenuItem
      Caption = #1054'&'#1090#1095#1077#1090#1099
      GroupIndex = 8
      object N20: TMenuItem
        Action = acCorrections
      end
    end
    object Window1: TMenuItem
      Caption = #1054'&'#1082#1085#1086
      GroupIndex = 10
      object ArrangeAll1: TMenuItem
        Action = WindowClose1
      end
      object N11: TMenuItem
        Caption = '-'
      end
    end
    object N12: TMenuItem
      Caption = #1057'&'#1087#1088#1072#1074#1082#1072
      GroupIndex = 11
      object N13: TMenuItem
        Action = acAbout
      end
    end
  end
  object ActionList: TActionList
    Images = ImageList1
    Left = 96
    Top = 152
    object acStorage: TAction
      Category = 'Storage'
      Caption = #1054'&'#1089#1090#1072#1090#1082#1080
      Hint = #1054#1089#1090#1072#1090#1082#1080
      OnExecute = acStorageExecute
    end
    object acClose: TAction
      Category = 'File'
      Caption = '&'#1047#1072#1082#1088#1099#1090#1100
      Hint = #1047#1072#1074#1077#1088#1096#1080#1090#1100' '#1087#1088#1080#1083#1086#1078#1077#1085#1080#1077
      OnExecute = acCloseExecute
    end
    object acMealOperation: TAction
      Category = 'Storage'
      Caption = #1055#1088#1080#1093#1086#1076'/&'#1056#1072#1089#1093#1086#1076
      Hint = #1054#1087#1077#1088#1072#1094#1080#1080' '#1087#1088#1080#1093#1086#1076#1072' '#1080' '#1088#1072#1089#1093#1086#1076#1072
      ImageIndex = 1
      OnExecute = acMealOperationExecute
    end
    object acDishesSelling: TAction
      Category = 'Storage'
      Caption = '&'#1041#1083#1102#1076#1072
      Hint = #1055#1088#1086#1076#1072#1085#1085#1099#1077' '#1073#1083#1102#1076#1072
      ImageIndex = 0
      OnExecute = acDishesSellingExecute
    end
    object acMealSelling: TAction
      Category = 'Storage'
      Caption = '&'#1055#1088#1086#1076#1091#1082#1090#1099
      Hint = #1055#1088#1086#1076#1072#1085#1085#1099#1077' '#1087#1088#1086#1076#1091#1082#1090#1099
      OnExecute = acMealSellingExecute
    end
    object acDishTypes: TAction
      Category = 'Dictionaries'
      Caption = #1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1073'&'#1083#1102#1076
      Hint = #1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1073#1083#1102#1076
      OnExecute = acDishTypesExecute
    end
    object acDishes: TAction
      Category = 'Dictionaries'
      Caption = '&'#1041#1083#1102#1076#1072
      Hint = #1041#1083#1102#1076#1072
      OnExecute = acDishesExecute
    end
    object acMealTypes: TAction
      Category = 'Dictionaries'
      Caption = #1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1087'&'#1088#1086#1076#1091#1082#1090#1086#1074
      Hint = #1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1087#1088#1086#1076#1091#1082#1090#1086#1074
      OnExecute = acMealTypesExecute
    end
    object WindowClose1: TWindowClose
      Category = 'Window'
      Caption = '&'#1047#1072#1082#1088#1099#1090#1100
      Hint = #1047#1072#1082#1088#1099#1090#1100
    end
    object acAbout: TAction
      Category = 'Help'
      Caption = '&'#1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077'...'
      Hint = #1048#1085#1092#1086#1088#1084#1072#1094#1080#1103' '#1086' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      OnExecute = acAboutExecute
    end
    object acMeal: TAction
      Category = 'Dictionaries'
      Caption = '&'#1055#1088#1086#1076#1091#1082#1090#1099
      Hint = #1055#1088#1086#1076#1091#1082#1090#1099
      OnExecute = acMealExecute
    end
    object acMoveRests: TAction
      Category = 'Storage'
      Caption = #1055#1077'&'#1088#1077#1085#1077#1089#1090#1080' '#1086#1089#1090#1072#1090#1082#1080'...'
      Hint = #1055#1077#1088#1077#1085#1077#1089#1090#1080' '#1086#1089#1090#1072#1090#1082#1080' '#1085#1072' '#1085#1072#1095#1072#1083#1086' '#1084#1077#1089#1103#1094#1072
      OnExecute = acMoveRestsExecute
    end
    object acMoving: TAction
      Category = 'Storage'
      Caption = #1054'&'#1073#1086#1088#1086#1090#1099'...'
      Hint = #1054#1073#1086#1088#1086#1090#1099' '#1085#1072' '#1095#1080#1089#1083#1086' '#1080#1083#1080' '#1079#1072' '#1087#1077#1088#1080#1086#1076
      ImageIndex = 2
      OnExecute = acMovingExecute
    end
    object asSetDate: TAction
      Category = 'Settings'
      Caption = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1100' &'#1076#1072#1090#1091'...'
      Hint = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1090#1077#1082#1091#1097#1091#1102' '#1076#1072#1090#1091
      OnExecute = asSetDateExecute
    end
    object acCorrections: TAction
      Category = 'Reports'
      Caption = #1055#1086' &'#1082#1091#1093#1085#1077'...'
      Hint = #1054#1090#1095#1077#1090' '#1087#1086' '#1082#1091#1093#1085#1077
      OnExecute = acCorrectionsExecute
    end
    object acDeleteData: TAction
      Category = 'Service'
      Caption = '&'#1059#1076#1072#1083#1080#1090#1100' '#1076#1072#1085#1085#1099#1077'...'
      Hint = #1059#1076#1072#1083#1077#1085#1080#1077' '#1074#1089#1077#1093' '#1076#1072#1085#1085#1099#1093' '#1076#1086' '#1091#1082#1072#1079#1072#1085#1085#1086#1081' '#1076#1072#1090#1099
      OnExecute = acDeleteDataExecute
    end
  end
  object ImageList1: TImageList
    Left = 228
    Top = 196
    Bitmap = {
      494C010103000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001001000000000000008
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000007C
      007C0000007C007C000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF7FFF7FFF7F00000000000000000000000000000000000000000000007C007C
      007C007C007C007C007C00000000000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF7F00000000000000000000000000000000000000000000007C007C007C
      007C007C007C007C007C007C0000000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF7F0000000000000000000000000000000000000000007C007C007C007C
      007C007C007C007C007C007C007C000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF7F0000000000000000000000000000000000000000007C007C007C007C
      007C007C007C007C007C007C007C000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF7F0000000000000000000000000000000000000000007C007C007C007C
      007C007C007C007C007C007C007C0000000000000000000000000000FF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0040007C0040000000000000000000000000000000000000007CFF7F007C007C
      007C007C007C007C007C007C007C0000000000000000000000000000FF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      007C0040007C000000000000000000000000000000000000007CFF7F007C007C
      007C007C007C007C007C007C007C0000000000000000000000000000FF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000007C
      0040007C0040007C000000000000000000000000000000000000007CFF7F007C
      007C007C007C007C007C007C0000000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000040
      007C0040007C00400000000000000000000000000000000000000000007C007C
      007C0002007C007C007C00000000000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF7FFF7F
      FF7FFF7FFF7FFF7FFF7F00000000000000000000000000020002000200020002
      0000000200000000000000000000000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000FF7FFF7F
      FF7FFF7FFF7FFF7FFF7F00000000000000000000000000000002000200020002
      0002000200000002000200020002000200000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7F0000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF7FFF7FFF7F
      FF7FFF7FFF7FFF7FFF7FFF7F0000000000000000000000000000000200020002
      0000000200020002000200020002000000000000000000000000000000000000
      0000000000000000104200000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000002000000020002000200000000000000000000000000000000FF7FFF7F
      FF7FFF7FFF7FFF7F000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0002000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFFFFFF0000F007FC1FE01F0000
      FC1FF80FE00F0000FE3FF007E0070000FE3FE003E0070000FE3FE003E0070000
      FE3FE003E0070000FC1FE003E0070000FC1FE003E0070000F80FF007E0070000
      F80FF80FE0070000F007C17FE0070000F007E041E0070000E003F103E0070000
      E003FF47F0070000FFFFFEFFF807000000000000000000000000000000000000
      000000000000}
  end
  object AppEvents: TApplicationEvents
    Left = 220
    Top = 80
  end
  object menuSelling: TPopupMenu
    Images = ImageList1
    Left = 104
    Top = 80
    object N24: TMenuItem
      Action = acDishesSelling
    end
    object N25: TMenuItem
      Action = acMealSelling
    end
  end
end
