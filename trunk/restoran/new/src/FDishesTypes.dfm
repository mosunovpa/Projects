object frmDishesTypes: TfrmDishesTypes
  Left = 21
  Top = 134
  Width = 544
  Height = 367
  Caption = #1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1073#1083#1102#1076
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
    Caption = ' '#1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1073#1083#1102#1076':'
    TabOrder = 0
    object navDishesTypes: TDBNavigatorEx
      Left = 306
      Top = 0
      Width = 230
      Height = 22
      DataSource = dsDishesTypes
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
    Height = 318
    Align = alClient
    DataSource = dsDishesTypes
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
  object tblDishesTypes: TADOTable
    Connection = dmodMain.connDB
    BeforePost = tblDishesTypesBeforePost
    OnDeleteError = tblDishesTypesDeleteError
    TableName = 'DishesTypes'
    Left = 48
    Top = 136
    object tblDishesTypesId: TAutoIncField
      FieldName = 'Id'
      ReadOnly = True
      Visible = False
    end
    object tblDishesTypesName: TWideStringField
      DisplayLabel = #1050#1072#1090#1077#1075#1086#1088#1080#1103
      DisplayWidth = 50
      FieldName = 'Name'
      Size = 255
    end
  end
  object dsDishesTypes: TDataSource
    DataSet = tblDishesTypes
    Left = 152
    Top = 136
  end
end
