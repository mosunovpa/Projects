object dmodMain: TdmodMain
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Left = 269
  Top = 208
  Height = 163
  Width = 224
  object connDB: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=D:\projects\restora' +
      'n\new\src\data\restoran.mdb;Persist Security Info=False'
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 20
    Top = 24
  end
end
