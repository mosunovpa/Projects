object dmodRestCalc: TdmodRestCalc
  OldCreateOrder = False
  Left = 432
  Top = 111
  Height = 173
  Width = 215
  object qryRest: TADOQuery
    Connection = dmodMain.connDB
    Parameters = <>
    Prepared = True
    Left = 16
    Top = 12
  end
  object qry: TADOQuery
    Connection = dmodMain.connDB
    Parameters = <>
    Left = 88
    Top = 8
  end
end
