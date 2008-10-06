object frmMain: TfrmMain
  Left = 437
  Top = 127
  Width = 737
  Height = 461
  Caption = #1040#1087#1090#1077#1082#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    000000008000008000000080800080000000800080008080000080808000C0C0
    C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000008888800000000000000000000000008844444880000000000000000
    0000008447777744800000000000000000000844444444444800000000000000
    0000844444444444448000000000000000008444444444444480000000000000
    000844444E444444444800000000000000084444E44444444448000000000000
    0008444E6E44444444480000000000000008444CECECCCCC4448000000000000
    00084CCE6ECCCCCCCC4800000000000000008CCCE6ECCCCCCC80000000000000
    000088FCCECCCCCCF8800000000000000000088F8F8F8F8F8800000000000000
    00000088FFF8F8F88000000000000000000000088FFF8F880000000000000000
    0000000088FFF8800000000000000000000000007F8F8F700000000000000000
    0000000008FFF800000000000000000000000000088F88000000000000000000
    0000000008FFF800000000000000000000000000088F88000000000000000000
    0000000008FFF800000000000000000000000000088F88000000000000000000
    0000000008F87700000000000000000000000007888888870000000000000000
    0000000887777788000000000000000000000007788888770000000000000000
    000000000000000000000000000000000000000000000000000000000000FFFF
    FFFFFFF83FFFFFE00FFFFFC007FFFF8003FFFF0001FFFE0000FFFE0000FFFC00
    007FFC00007FFC00007FFC00007FFC00007FFE0000FFFE0000FFFF0001FFFF80
    03FFFFC007FFFFE00FFFFFE00FFFFFF01FFFFFF01FFFFFF01FFFFFF01FFFFFF0
    1FFFFFF01FFFFFE00FFFFFC007FFFFC007FFFFC007FFFFE00FFFFFFFFFFF}
  KeyPreview = True
  Menu = mnuMain
  OldCreateOrder = True
  Position = poDefault
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Pages: TPageControl
    Left = 0
    Top = 0
    Width = 729
    Height = 407
    ActivePage = tsPatient
    Align = alClient
    TabOrder = 0
    object tsPatient: TTabSheet
      Caption = #1055#1072#1094#1080#1077#1085#1090#1099
      object splitPatient: TSplitter
        Left = 0
        Top = 209
        Width = 721
        Height = 3
        Cursor = crVSplit
        Align = alTop
      end
      object panelPatientBottom: TPanel
        Left = 0
        Top = 212
        Width = 721
        Height = 167
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object Panel5: TPanel
          Left = 0
          Top = 0
          Width = 721
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object btnAdd: TSpeedButton
            Left = 0
            Top = 3
            Width = 121
            Height = 21
            Hint = #1053#1072#1079#1085#1072#1095#1080#1090#1100' '#1085#1086#1074#1099#1081' '#1087#1088#1077#1087#1072#1088#1072#1090' '#1091#1082#1072#1079#1072#1085#1085#1086#1084#1091' '#1087#1072#1094#1080#1077#1085#1090#1091
            Caption = #1053#1072#1079#1085#1072#1095#1080#1090#1100' (F7)...'
            Flat = True
            Glyph.Data = {
              76010000424D7601000000000000760000002800000020000000100000000100
              0400000000000001000000000000000000001000000010000000000000000000
              800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
              33333333FF33333333FF333993333333300033377F3333333777333993333333
              300033F77FFF3333377739999993333333333777777F3333333F399999933333
              33003777777333333377333993333333330033377F3333333377333993333333
              3333333773333333333F333333333333330033333333F33333773333333C3333
              330033333337FF3333773333333CC333333333FFFFF77FFF3FF33CCCCCCCCCC3
              993337777777777F77F33CCCCCCCCCC3993337777777777377333333333CC333
              333333333337733333FF3333333C333330003333333733333777333333333333
              3000333333333333377733333333333333333333333333333333}
            NumGlyphs = 2
            ParentShowHint = False
            ShowHint = True
            OnClick = btnAddClick
          end
          object Bevel1: TBevel
            Left = 0
            Top = 0
            Width = 721
            Height = 3
            Align = alTop
            Shape = bsTopLine
          end
          object btnDosa: TSpeedButton
            Left = 208
            Top = 3
            Width = 73
            Height = 21
            Hint = #1059#1082#1072#1079#1072#1090#1100' '#1076#1086#1079#1091' '#1085#1072#1079#1085#1072#1095#1077#1085#1085#1086#1075#1086' '#1087#1088#1077#1087#1072#1088#1072#1090#1072
            Caption = #1044#1086#1079#1072'...'
            Flat = True
            Glyph.Data = {
              76010000424D7601000000000000760000002800000020000000100000000100
              0400000000000001000000000000000000001000000010000000000000000000
              800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000000
              000033333377777777773333330FFFFFFFF03FF3FF7FF33F3FF700300000FF0F
              00F077F777773F737737E00BFBFB0FFFFFF07773333F7F3333F7E0BFBF000FFF
              F0F077F3337773F3F737E0FBFBFBF0F00FF077F3333FF7F77F37E0BFBF00000B
              0FF077F3337777737337E0FBFBFBFBF0FFF077F33FFFFFF73337E0BF0000000F
              FFF077FF777777733FF7000BFB00B0FF00F07773FF77373377373330000B0FFF
              FFF03337777373333FF7333330B0FFFF00003333373733FF777733330B0FF00F
              0FF03333737F37737F373330B00FFFFF0F033337F77F33337F733309030FFFFF
              00333377737FFFFF773333303300000003333337337777777333}
            NumGlyphs = 2
            ParentShowHint = False
            ShowHint = True
            OnClick = btnDosaClick
          end
          object btnDel: TSpeedButton
            Left = 121
            Top = 3
            Width = 87
            Height = 21
            Hint = #1054#1090#1084#1077#1085#1080#1090#1100' '#1085#1072#1079#1085#1072#1095#1077#1085#1080#1077' '#1091#1082#1072#1079#1072#1085#1085#1086#1075#1086' '#1087#1088#1077#1087#1072#1088#1072#1090#1072
            Caption = #1054#1090#1084#1077#1085#1080#1090#1100
            Flat = True
            Glyph.Data = {
              76010000424D7601000000000000760000002800000020000000100000000100
              0400000000000001000000000000000000001000000010000000000000000000
              800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
              333333333333333333FF33333333333330003333333333333777333333333333
              300033FFFFFF3333377739999993333333333777777F3333333F399999933333
              3300377777733333337733333333333333003333333333333377333333333333
              3333333333333333333F333333333333330033333F33333333773333C3333333
              330033337F3333333377333CC3333333333333F77FFFFFFF3FF33CCCCCCCCCC3
              993337777777777F77F33CCCCCCCCCC399333777777777737733333CC3333333
              333333377F33333333FF3333C333333330003333733333333777333333333333
              3000333333333333377733333333333333333333333333333333}
            NumGlyphs = 2
            ParentShowHint = False
            ShowHint = True
            OnClick = btnDelClick
          end
          object btnGotoMedicine: TSpeedButton
            Left = 281
            Top = 3
            Width = 120
            Height = 21
            Hint = #1053#1072#1081#1090#1080' '#1085#1072#1079#1085#1072#1095#1077#1085#1085#1099#1081' '#1087#1088#1077#1087#1072#1088#1072#1090
            Caption = #1053#1072#1081#1090#1080' '#1087#1088#1077#1087#1072#1088#1072#1090
            Flat = True
            Glyph.Data = {
              76010000424D7601000000000000760000002800000020000000100000000100
              0400000000000001000000000000000000001000000010000000000000000000
              800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
              3333333333333333333333333333333333333FFF333333333333000333333333
              3333777FFF3FFFFF33330B000300000333337F777F777773F333000E00BFBFB0
              3333777F773333F7F333000E0BFBF0003333777F7F3337773F33000E0FBFBFBF
              0333777F7F3333FF7FFF000E0BFBF0000003777F7F3337777773000E0FBFBFBF
              BFB0777F7F33FFFFFFF7000E0BF000000003777F7FF777777773000000BFB033
              33337777773FF733333333333300033333333333337773333333333333333333
              3333333333333333333333333333333333333333333333333333333333333333
              3333333333333333333333333333333333333333333333333333}
            NumGlyphs = 2
            ParentShowHint = False
            ShowHint = True
            OnClick = btnGotoMedicineClick
          end
        end
        object panelPatientMedicine: TPanel
          Left = 0
          Top = 25
          Width = 721
          Height = 20
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvLowered
          Caption = #1053#1072#1079#1085#1072#1095#1077#1085#1085#1099#1077' '#1087#1088#1077#1087#1072#1088#1072#1090#1099':'
          TabOrder = 1
        end
        object gridPatientMedicine: TDBGrid
          Left = 0
          Top = 45
          Width = 721
          Height = 122
          Align = alClient
          DataSource = dsPatientMedicine
          Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          TabOrder = 2
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          OnDblClick = gridPatientMedicineDblClick
          Columns = <
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Name'
              Title.Caption = #1052#1077#1078#1076'.'#1085#1072#1080#1084'.'
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Sellname'
              Title.Caption = #1058#1086#1088#1075'.'#1085#1072#1080#1084'.,'#1076#1086#1079'.,'#1092'.'#1074'.'
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Dosa'
              Title.Caption = #1056#1072#1079'.'#1076#1086#1079#1072', '#1082#1088'-'#1090#1100' '#1087#1088#1080#1077#1084#1072
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Quantity'
              Title.Caption = #1082#1086#1083'-'#1074#1086' '#1091#1087'. '#1074' '#1084#1077#1089'.'
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Description'
              Title.Caption = #1054#1073#1086#1089#1085#1086#1074#1072#1085#1080#1077
              Visible = True
            end>
        end
      end
      object panelPatientTop: TPanel
        Left = 0
        Top = 0
        Width = 721
        Height = 209
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object gridPatient: TDBGrid
          Left = 0
          Top = 42
          Width = 721
          Height = 167
          Align = alClient
          DataSource = dsPatient
          Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          TabOrder = 2
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          Columns = <
            item
              Expanded = False
              FieldName = 'Lastname'
              Title.Caption = #1060#1072#1084#1080#1083#1080#1103
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Firstname'
              Title.Caption = #1048#1084#1103
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Secondname'
              Title.Caption = #1054#1090#1095#1077#1089#1090#1074#1086
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Address'
              Title.Caption = #1040#1076#1088#1077#1089
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Birthday'
              Title.Caption = #1044#1072#1090#1072' '#1088#1086#1078#1076#1077#1085#1080#1103
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Code'
              Title.Caption = #1050#1086#1076' '#1083#1100#1075#1086#1090#1099
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Invalid'
              Title.Caption = #1048#1085#1074#1072#1083#1080#1076#1085#1086#1089#1090#1100' ('#1076#1072#1090#1072' '#1091#1089#1090'.)'
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Diagnos'
              Title.Caption = #1044#1080#1072#1075#1085#1086#1079
              Visible = True
            end>
        end
        object Panel2: TPanel
          Left = 0
          Top = 22
          Width = 721
          Height = 20
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvLowered
          Caption = #1055#1072#1094#1080#1077#1085#1090#1099':'
          TabOrder = 0
        end
        object Panel7: TPanel
          Left = 0
          Top = 0
          Width = 721
          Height = 22
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object DBNavigator1: TDBNavigator
            Left = 128
            Top = 0
            Width = 198
            Height = 21
            DataSource = dsPatient
            VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel]
            Flat = True
            Hints.Strings = (
              #1055#1077#1088#1074#1099#1081
              #1055#1088#1077#1076#1099#1076#1091#1097#1080#1081
              #1057#1083#1077#1076#1091#1102#1097#1080#1081
              #1055#1086#1089#1083#1077#1076#1085#1080#1081
              #1044#1086#1073#1072#1074#1080#1090#1100' (Ins)'
              #1059#1076#1072#1083#1080#1090#1100' (Ctrl+Del)'
              #1048#1079#1084#1077#1085#1080#1090#1100' (F2)'
              #1055#1088#1080#1084#1077#1085#1080#1090#1100
              #1054#1090#1084#1077#1085#1080#1090#1100' (Esc)'
              #1054#1073#1085#1086#1074#1080#1090#1100)
            ParentShowHint = False
            ShowHint = True
            TabOrder = 0
          end
          object edPatientSearch: TEdit
            Left = 0
            Top = 0
            Width = 121
            Height = 21
            Hint = #1055#1086#1080#1089#1082' '#1087#1072#1094#1080#1077#1085#1090#1072' '#1087#1086' '#1092#1072#1084#1080#1083#1080#1080
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
            OnKeyDown = edPatientSearchKeyDown
            OnKeyPress = edPatientSearchKeyPress
          end
        end
      end
    end
    object tsMedicine: TTabSheet
      Caption = #1055#1088#1077#1087#1072#1088#1072#1090#1099
      object panelMedicineBottom: TPanel
        Left = 0
        Top = 0
        Width = 721
        Height = 379
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object Splitter1: TSplitter
          Left = 0
          Top = 185
          Width = 721
          Height = 3
          Cursor = crVSplit
          Align = alTop
        end
        object Panel6: TPanel
          Left = 0
          Top = 188
          Width = 721
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          object Bevel2: TBevel
            Left = 0
            Top = 0
            Width = 721
            Height = 3
            Align = alTop
            Shape = bsTopLine
          end
          object btnDosa2: TSpeedButton
            Left = 0
            Top = 3
            Width = 73
            Height = 21
            Hint = #1059#1082#1072#1079#1072#1090#1100' '#1076#1086#1079#1091' '#1085#1072#1079#1085#1072#1095#1077#1085#1085#1086#1075#1086' '#1087#1088#1077#1087#1072#1088#1072#1090#1072
            Caption = #1044#1086#1079#1072'...'
            Flat = True
            Glyph.Data = {
              76010000424D7601000000000000760000002800000020000000100000000100
              0400000000000001000000000000000000001000000010000000000000000000
              800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000000
              000033333377777777773333330FFFFFFFF03FF3FF7FF33F3FF700300000FF0F
              00F077F777773F737737E00BFBFB0FFFFFF07773333F7F3333F7E0BFBF000FFF
              F0F077F3337773F3F737E0FBFBFBF0F00FF077F3333FF7F77F37E0BFBF00000B
              0FF077F3337777737337E0FBFBFBFBF0FFF077F33FFFFFF73337E0BF0000000F
              FFF077FF777777733FF7000BFB00B0FF00F07773FF77373377373330000B0FFF
              FFF03337777373333FF7333330B0FFFF00003333373733FF777733330B0FF00F
              0FF03333737F37737F373330B00FFFFF0F033337F77F33337F733309030FFFFF
              00333377737FFFFF773333303300000003333337337777777333}
            NumGlyphs = 2
            ParentShowHint = False
            ShowHint = True
            OnClick = btnDosa2Click
          end
          object btnGotoPatient: TSpeedButton
            Left = 73
            Top = 3
            Width = 120
            Height = 21
            Hint = #1053#1072#1081#1090#1080' '#1091#1082#1072#1079#1072#1085#1085#1086#1075#1086' '#1087#1072#1094#1080#1077#1085#1090#1072
            Caption = #1053#1072#1081#1090#1080' '#1087#1072#1094#1080#1077#1085#1090#1072
            Flat = True
            Glyph.Data = {
              76010000424D7601000000000000760000002800000020000000100000000100
              0400000000000001000000000000000000001000000010000000000000000000
              800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
              3333333333333333333333333333333333333FFF333333333333000333333333
              3333777FFF3FFFFF33330B000300000333337F777F777773F333000E00BFBFB0
              3333777F773333F7F333000E0BFBF0003333777F7F3337773F33000E0FBFBFBF
              0333777F7F3333FF7FFF000E0BFBF0000003777F7F3337777773000E0FBFBFBF
              BFB0777F7F33FFFFFFF7000E0BF000000003777F7FF777777773000000BFB033
              33337777773FF733333333333300033333333333337773333333333333333333
              3333333333333333333333333333333333333333333333333333333333333333
              3333333333333333333333333333333333333333333333333333}
            NumGlyphs = 2
            ParentShowHint = False
            ShowHint = True
            OnClick = btnGotoPatientClick
          end
        end
        object panelMedicinePatient: TPanel
          Left = 0
          Top = 213
          Width = 721
          Height = 20
          Align = alTop
          Alignment = taLeftJustify
          BevelOuter = bvLowered
          Caption = #1050#1086#1084#1091' '#1085#1072#1079#1085#1072#1095#1077#1085':'
          TabOrder = 1
        end
        object gridMedicinePatient: TDBGrid
          Left = 0
          Top = 233
          Width = 721
          Height = 146
          Align = alClient
          DataSource = dsMedicinePatient
          Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
          TabOrder = 2
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
          OnDblClick = gridMedicinePatientDblClick
          Columns = <
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Lastname'
              Title.Caption = #1060#1072#1084#1080#1083#1080#1103
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Firstname'
              Title.Caption = #1048#1084#1103
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Secondname'
              Title.Caption = #1054#1090#1095#1077#1089#1090#1074#1086
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Address'
              Title.Caption = #1040#1076#1088#1077#1089
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Birthday'
              Title.Caption = #1044#1072#1090#1072' '#1088#1086#1078#1076#1077#1085#1080#1103
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Code'
              Title.Caption = #1050#1086#1076' '#1083#1100#1075#1086#1090#1099
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Invalid'
              Title.Caption = #1048#1085#1074#1072#1083#1080#1076#1085#1086#1089#1090#1100' ('#1076#1072#1090#1072' '#1091#1089#1090'.)'
              Visible = True
            end
            item
              Color = cl3DLight
              Expanded = False
              FieldName = 'Diagnos'
              Title.Caption = #1044#1080#1072#1075#1085#1086#1079
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Dosa'
              Title.Caption = #1056#1072#1079'.'#1076#1086#1079#1072', '#1082#1088'-'#1090#1100' '#1087#1088#1080#1077#1084#1072
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Quantity'
              Title.Caption = #1082#1086#1083'-'#1074#1086' '#1091#1087'. '#1074' '#1084#1077#1089'.'
              Visible = True
            end
            item
              Expanded = False
              FieldName = 'Description'
              Title.Caption = #1054#1073#1086#1089#1085#1086#1074#1072#1085#1080#1077
              Visible = True
            end>
        end
        object panelMedicine: TPanel
          Left = 0
          Top = 0
          Width = 721
          Height = 185
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 3
          object Splitter2: TSplitter
            Left = 384
            Top = 22
            Height = 163
          end
          object panelMedicineLeft: TPanel
            Left = 0
            Top = 22
            Width = 384
            Height = 163
            Align = alLeft
            BevelOuter = bvNone
            Caption = 'panelMedicineLeft'
            TabOrder = 1
            object gridMedicine: TDBGrid
              Left = 0
              Top = 20
              Width = 384
              Height = 143
              Align = alClient
              DataSource = dsMedicine
              Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
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
                  Title.Caption = #1052#1077#1078#1076'.'#1085#1072#1080#1084'.'
                  Visible = True
                end
                item
                  Expanded = False
                  FieldName = 'Sellname'
                  Title.Caption = #1058#1086#1088#1075'.'#1085#1072#1080#1084'.,'#1076#1086#1079'.,'#1092'.'#1074'.'
                  Visible = True
                end>
            end
            object Panel3: TPanel
              Left = 0
              Top = 0
              Width = 384
              Height = 20
              Align = alTop
              Alignment = taLeftJustify
              BevelOuter = bvLowered
              Caption = #1055#1088#1077#1087#1072#1088#1072#1090#1099':'
              TabOrder = 1
            end
          end
          object Panel8: TPanel
            Left = 0
            Top = 0
            Width = 721
            Height = 22
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 0
            object Bevel3: TBevel
              Left = 395
              Top = 0
              Width = 3
              Height = 20
              Shape = bsLeftLine
            end
            object btnSave: TSpeedButton
              Left = 577
              Top = 0
              Width = 89
              Height = 21
              Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1086#1090#1095#1077#1090
              Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
              Flat = True
              Glyph.Data = {
                76010000424D7601000000000000760000002800000020000000100000000100
                0400000000000001000000000000000000001000000010000000000000000000
                800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
                333333FFFFFFFFFFFFF33000077777770033377777777777773F000007888888
                00037F3337F3FF37F37F00000780088800037F3337F77F37F37F000007800888
                00037F3337F77FF7F37F00000788888800037F3337777777337F000000000000
                00037F3FFFFFFFFFFF7F00000000000000037F77777777777F7F000FFFFFFFFF
                00037F7F333333337F7F000FFFFFFFFF00037F7F333333337F7F000FFFFFFFFF
                00037F7F333333337F7F000FFFFFFFFF00037F7F333333337F7F000FFFFFFFFF
                00037F7F333333337F7F000FFFFFFFFF07037F7F33333333777F000FFFFFFFFF
                0003737FFFFFFFFF7F7330099999999900333777777777777733}
              NumGlyphs = 2
              ParentShowHint = False
              ShowHint = True
              OnClick = btnSaveClick
            end
            object btnSelect: TSpeedButton
              Left = 403
              Top = 0
              Width = 78
              Height = 21
              Hint = #1042#1099#1073#1088#1072#1090#1100' '#1087#1088#1077#1087#1072#1088#1072#1090' '#1074' '#1086#1090#1095#1077#1090
              Caption = #1042#1099#1073#1088#1072#1090#1100
              Flat = True
              Glyph.Data = {
                76010000424D7601000000000000760000002800000020000000100000000100
                0400000000000001000000000000000000001000000010000000000000000000
                800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
                3333333333333333333333333333333333333333333333333333333333333333
                3333333333333333333333333333333333333333333FF3333333333333003333
                3333333333773FF3333333333309003333333333337F773FF333333333099900
                33333FFFFF7F33773FF30000000999990033777777733333773F099999999999
                99007FFFFFFF33333F7700000009999900337777777F333F7733333333099900
                33333333337F3F77333333333309003333333333337F77333333333333003333
                3333333333773333333333333333333333333333333333333333333333333333
                3333333333333333333333333333333333333333333333333333}
              NumGlyphs = 2
              ParentShowHint = False
              ShowHint = True
              OnClick = btnSelectClick
            end
            object btnDeselect: TSpeedButton
              Left = 481
              Top = 0
              Width = 69
              Height = 21
              Hint = #1059#1073#1088#1072#1090#1100' '#1087#1088#1077#1087#1072#1088#1072#1090' '#1080#1079' '#1086#1090#1095#1077#1090#1072
              Caption = #1059#1073#1088#1072#1090#1100
              Flat = True
              Glyph.Data = {
                76010000424D7601000000000000760000002800000020000000100000000100
                0400000000000001000000000000000000001000000010000000000000000000
                800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
                3333333333333333333333333333333333333333333333333333333333333333
                3333333333333FF3333333333333003333333333333F77F33333333333009033
                333333333F7737F333333333009990333333333F773337FFFFFF330099999000
                00003F773333377777770099999999999990773FF33333FFFFF7330099999000
                000033773FF33777777733330099903333333333773FF7F33333333333009033
                33333333337737F3333333333333003333333333333377333333333333333333
                3333333333333333333333333333333333333333333333333333333333333333
                3333333333333333333333333333333333333333333333333333}
              NumGlyphs = 2
              ParentShowHint = False
              ShowHint = True
              OnClick = btnDeselectClick
            end
            object btnCount: TSpeedButton
              Left = 550
              Top = 0
              Width = 27
              Height = 21
              Hint = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1091#1087#1072#1082#1086#1074#1086#1082
              Flat = True
              Glyph.Data = {
                76010000424D7601000000000000760000002800000020000000100000000100
                0400000000000001000000000000000000001000000010000000000000000000
                800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
                FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000000
                000033333377777777773333330FFFFFFFF03FF3FF7FF33F3FF700300000FF0F
                00F077F777773F737737E00BFBFB0FFFFFF07773333F7F3333F7E0BFBF000FFF
                F0F077F3337773F3F737E0FBFBFBF0F00FF077F3333FF7F77F37E0BFBF00000B
                0FF077F3337777737337E0FBFBFBFBF0FFF077F33FFFFFF73337E0BF0000000F
                FFF077FF777777733FF7000BFB00B0FF00F07773FF77373377373330000B0FFF
                FFF03337777373333FF7333330B0FFFF00003333373733FF777733330B0FF00F
                0FF03333737F37737F373330B00FFFFF0F033337F77F33337F733309030FFFFF
                00333377737FFFFF773333303300000003333337337777777333}
              NumGlyphs = 2
              ParentShowHint = False
              ShowHint = True
              OnClick = btnCountClick
            end
            object DBNavigator2: TDBNavigator
              Left = 168
              Top = 0
              Width = 198
              Height = 21
              DataSource = dsMedicine
              VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel]
              Flat = True
              Hints.Strings = (
                #1055#1077#1088#1074#1099#1081
                #1055#1088#1077#1076#1099#1076#1091#1097#1080#1081
                #1057#1083#1077#1076#1091#1102#1097#1080#1081
                #1055#1086#1089#1083#1077#1076#1085#1080#1081
                #1044#1086#1073#1072#1074#1080#1090#1100' (Ins)'
                #1059#1076#1072#1083#1080#1090#1100' (Ctrl+Del)'
                #1048#1079#1084#1077#1085#1080#1090#1100' (F2)'
                #1055#1088#1080#1084#1077#1085#1080#1090#1100
                #1054#1090#1084#1077#1085#1080#1090#1100' (Esc)'
                #1054#1073#1085#1086#1074#1080#1090#1100)
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
            end
            object edMedicineSearch: TEdit
              Left = 64
              Top = 0
              Width = 97
              Height = 21
              Hint = #1055#1086#1080#1089#1082' '#1087#1086' '#1085#1072#1080#1084#1077#1085#1086#1074#1072#1085#1080#1102' '#1087#1088#1077#1087#1072#1088#1072#1090#1072
              ParentShowHint = False
              ShowHint = True
              TabOrder = 1
              OnKeyDown = edMedicineSearchKeyDown
              OnKeyPress = edMedicineSearchKeyPress
            end
            object cmbSort: TComboBox
              Left = 0
              Top = 0
              Width = 64
              Height = 21
              Hint = #1057#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1087#1086' '#1084#1077#1078#1076#1091#1085#1072#1088#1086#1076#1085#1086#1084#1091' / '#1090#1086#1088#1075#1086#1074#1086#1084#1091' '#1085#1072#1080#1084#1077#1085#1086#1074#1072#1085#1080#1102
              Style = csDropDownList
              ItemHeight = 13
              ParentShowHint = False
              ShowHint = True
              TabOrder = 2
              OnChange = cmbSortChange
              Items.Strings = (
                #1052#1077#1078#1076'.'
                #1058#1086#1088#1075'.')
            end
          end
          object panelMedicineRight: TPanel
            Left = 387
            Top = 22
            Width = 334
            Height = 163
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 2
            object Panel1: TPanel
              Left = 0
              Top = 0
              Width = 334
              Height = 20
              Align = alTop
              Alignment = taLeftJustify
              BevelOuter = bvLowered
              Caption = #1042#1099#1073#1088#1072#1085#1085#1099#1077' '#1087#1088#1077#1087#1072#1088#1072#1090#1099':'
              TabOrder = 0
            end
            object gridSelectedMedicine: TDBGrid
              Left = 0
              Top = 20
              Width = 334
              Height = 143
              Align = alClient
              DataSource = dsSelectedMedicine
              Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
              TabOrder = 1
              TitleFont.Charset = DEFAULT_CHARSET
              TitleFont.Color = clWindowText
              TitleFont.Height = -11
              TitleFont.Name = 'MS Sans Serif'
              TitleFont.Style = []
              OnDblClick = gridSelectedMedicineDblClick
              OnKeyDown = gridSelectedMedicineKeyDown
              Columns = <
                item
                  Expanded = False
                  FieldName = 'Name'
                  Title.Caption = #1052#1077#1078#1076'.'#1085#1072#1080#1084'.'
                  Width = 83
                  Visible = True
                end
                item
                  Expanded = False
                  FieldName = 'Sellname'
                  Title.Caption = #1058#1086#1088#1075'.'#1085#1072#1080#1084'.,'#1076#1086#1079'.,'#1092'.'#1074'.'
                  Width = 96
                  Visible = True
                end
                item
                  Expanded = False
                  FieldName = 'Count'
                  Title.Caption = #1050#1086#1083'-'#1074#1086' '#1091#1087#1072#1082#1086#1074#1086#1082
                  Visible = True
                end>
            end
          end
        end
      end
    end
  end
  object dsPatient: TDataSource
    DataSet = tblAdoPatient
    Left = 176
    Top = 80
  end
  object mnuMain: TMainMenu
    Left = 80
    Top = 184
    object File1: TMenuItem
      Caption = #1060#1072#1081#1083
      object mnuExit: TMenuItem
        Caption = #1042#1099#1093#1086#1076
        OnClick = mnuExitClick
      end
    end
  end
  object dsMedicine: TDataSource
    DataSet = tblAdoMedicine
    Left = 332
    Top = 104
  end
  object dsPatientMedicine: TDataSource
    DataSet = qryAdoPatientMedicine
    Left = 268
    Top = 304
  end
  object dsMedicinePatient: TDataSource
    DataSet = qryAdoMedicinePatient
    Left = 460
    Top = 288
  end
  object timerSearch: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerSearchTimer
    Left = 20
    Top = 184
  end
  object dsSelectedMedicine: TDataSource
    DataSet = qryAdoSelectedMedicine
    Left = 543
    Top = 118
  end
  object savedlgReport: TSaveDialog
    DefaultExt = 'csv'
    FileName = 'report.csv'
    Filter = 'CSV ('#1088#1072#1079#1076#1077#1083#1080#1090#1077#1083#1080' - '#1079#1072#1087#1103#1090#1099#1077') (*.csv)|*.csv|'#1042#1089#1077' '#1092#1072#1081#1083#1099' (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly]
    Left = 132
    Top = 184
  end
  object AdoConnection: TADOConnection
    LoginPrompt = False
    Left = 36
    Top = 126
  end
  object tblAdoPatient: TADOTable
    Connection = AdoConnection
    AfterPost = tblPatientAfterPost
    AfterScroll = tblPatientAfterScroll
    OnDeleteError = tblAdoPatientDeleteError
    IndexFieldNames = 'Lastname'
    TableName = 'patient'
    Left = 116
    Top = 126
  end
  object tblAdoMedicine: TADOTable
    Connection = AdoConnection
    AfterPost = tblMedicineAfterPost
    AfterDelete = tblMedicineAfterDelete
    AfterScroll = tblMedicineAfterScroll
    OnDeleteError = tblAdoMedicineDeleteError
    IndexFieldNames = 'Name'
    TableName = 'medicine'
    Left = 268
    Top = 152
  end
  object tblAdoPatientMedicine: TADOTable
    Connection = AdoConnection
    TableName = 'patient_medicine'
    Left = 52
    Top = 348
  end
  object qryAdoPatientMedicine: TADOQuery
    Connection = AdoConnection
    DataSource = dsPatient
    Parameters = <
      item
        Name = 'id'
        DataType = ftInteger
        Value = Null
      end>
    SQL.Strings = (
      'select  * from patient_medicine pm, medicine m'
      'where m.id = pm.id_medicine and pm.id_patient = :id'
      'order by m.name')
    Left = 180
    Top = 356
  end
  object qryAdoMedicinePatient: TADOQuery
    Connection = AdoConnection
    DataSource = dsMedicine
    Parameters = <
      item
        Name = 'id'
        DataType = ftInteger
        Value = 0
      end>
    SQL.Strings = (
      'select  * from patient_medicine pm, patient p'
      'where p.id = pm.id_patient and pm.id_medicine = :id'
      'order by p.lastname')
    Left = 372
    Top = 340
  end
  object qryAdoSelectedMedicine: TADOQuery
    Connection = AdoConnection
    AfterScroll = qrySelectedMedicineAfterScroll
    Parameters = <>
    SQL.Strings = (
      
        'SELECT medicine.Id, medicine.Name , medicine.Sellname, selected_' +
        'medicine.Count'
      'FROM selected_medicine ,  medicine'
      'WHERE ( medicine.Id = selected_medicine.Id )'
      'order by medicine.Name')
    Left = 420
    Top = 176
  end
  object tblAdoSelectedMedicine: TADOTable
    Connection = AdoConnection
    TableName = 'selected_medicine'
    Left = 628
    Top = 184
  end
  object qryAdoReport: TADOQuery
    Connection = AdoConnection
    Parameters = <>
    SQL.Strings = (
      'SELECT patient.Lastname,'
      ' patient.Firstname,'
      ' patient.Secondname,'
      ' patient.Address,'
      ' patient.Birthday,'
      ' patient.Code,'
      ' patient.Invalid,'
      ' patient.Diagnos,'
      ' medicine.Name,'
      ' medicine.Sellname ,'
      ' patient_medicine.Dosa ,'
      ' patient_medicine.Quantity ,'
      ' patient_medicine.Description,'
      ' selected_medicine.Id,'
      ' selected_medicine.Count'
      'FROM patient, selected_medicine,'
      ' patient_medicine, medicine'
      'WHERE ( patient_medicine.Id_medicine = selected_medicine.Id )'
      '  AND'
      ' ( patient_medicine.Id_medicine = medicine.Id )'
      '  AND'
      ' ( patient_medicine.Id_patient = patient.Id )'
      'ORDER BY'
      ' medicine.Name, medicine.Sellname, patient.Lastname')
    Left = 564
    Top = 348
  end
  object qryAdoTemp: TADOQuery
    Connection = AdoConnection
    Parameters = <>
    Left = 660
    Top = 348
  end
end
