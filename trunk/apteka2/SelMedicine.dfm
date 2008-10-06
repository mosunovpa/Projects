object frmSelMedicine: TfrmSelMedicine
  Left = 428
  Top = 537
  BorderStyle = bsDialog
  Caption = #1040#1087#1090#1077#1082#1072
  ClientHeight = 359
  ClientWidth = 529
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = True
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 30
    Width = 103
    Height = 13
    Caption = #1042#1099#1073#1077#1088#1080#1090#1077' '#1087#1088#1077#1087#1072#1088#1072#1090':'
  end
  object lblPatient: TLabel
    Left = 8
    Top = 8
    Width = 43
    Height = 13
    Caption = 'lblPatient'
  end
  object gridSelMedicine: TDBGrid
    Left = 8
    Top = 51
    Width = 513
    Height = 270
    DataSource = dsSelMedicine
    Options = [dgEditing, dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgAlwaysShowSelection, dgConfirmDelete, dgCancelOnExit]
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
    OnDblClick = gridSelMedicineDblClick
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
  object btnOk: TButton
    Left = 368
    Top = 328
    Width = 75
    Height = 25
    Caption = #1053#1072#1079#1085#1072#1095#1080#1090#1100
    Default = True
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 448
    Top = 328
    Width = 75
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 3
    OnClick = btnCancelClick
  end
  object edSearch: TEdit
    Left = 193
    Top = 26
    Width = 104
    Height = 21
    TabOrder = 0
    OnKeyDown = edSearchKeyDown
    OnKeyPress = edSearchKeyPress
  end
  object cmbSort: TComboBox
    Left = 120
    Top = 26
    Width = 73
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 4
    OnChange = cmbSortChange
    Items.Strings = (
      #1052#1077#1078#1076'.'
      #1058#1086#1088#1075'.')
  end
  object DBNavigator1: TDBNavigator
    Left = 301
    Top = 26
    Width = 198
    Height = 22
    DataSource = dsSelMedicine
    VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast, nbInsert, nbDelete, nbEdit, nbPost, nbCancel]
    Flat = True
    TabOrder = 5
  end
  object dsSelMedicine: TDataSource
    DataSet = tblAdoSelMedicine
    Left = 224
    Top = 136
  end
  object timerSearch: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerSearchTimer
    Left = 344
    Top = 88
  end
  object tblAdoSelMedicine: TADOTable
    Connection = frmMain.AdoConnection
    AfterPost = tblAdoSelMedicineAfterPost
    AfterDelete = tblAdoSelMedicineAfterDelete
    OnDeleteError = tblAdoSelMedicineDeleteError
    IndexFieldNames = 'Name'
    TableName = 'medicine'
    Left = 88
    Top = 144
  end
end
