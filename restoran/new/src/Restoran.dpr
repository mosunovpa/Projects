program Restoran;

uses
  Forms,
  FMain in 'FMain.pas' {frmMain},
  DMMain in 'DMMain.pas' {dmodMain: TDataModule},
  FDishesTypes in 'FDishesTypes.pas' {frmDishesTypes},
  FMealTypes in 'FMealTypes.pas' {frmMealTypes},
  FDishes in 'FDishes.pas' {frmDishes},
  FMeal in 'FMeal.pas' {frmMeal},
  Storage in 'Storage.pas',
  MDIChildForm in 'MDIChildForm.pas',
  FOperation in 'FOperation.pas' {frmOperations},
  FDishSelling in 'FDishSelling.pas' {frmDishSelling},
  FStorage in 'FStorage.pas' {frmStorage},
  FMoving in 'FMoving.pas' {frmMoving},
  FAbout in 'FAbout.pas' {frmAbout},
  FSetDate in 'FSetDate.pas' {frmSetDate},
  FMovingPeriod in 'FMovingPeriod.pas' {frmMovingPeriod},
  GuiUtils in 'GuiUtils.pas',
  DBUtils in 'DBUtils.pas',
  DMRestCalc in 'DMRestCalc.pas' {dmodRestCalc: TDataModule},
  FMovingEx in 'FMovingEx.pas' {frmMovingEx},
  FSetMovingDate in 'FSetMovingDate.pas' {frmSetMovingDate},
  FLogin in 'FLogin.pas' {frmLogin},
  FCorrectRests in 'FCorrectRests.pas' {frmCorrectRests},
  FCorrectRep in 'FCorrectRep.pas' {frmCorrectRep},
  StrUtil in 'StrUtil.pas',
  DBFilter in 'DBFilter.pas',
  OperationFilter in 'OperationFilter.pas',
  Constants in 'Constants.pas',
  FMealSelling in 'FMealSelling.pas' {frmMealSelling},
  FSetMonth in 'FSetMonth.pas' {frmSetMonth};

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'Ресторан';
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TdmodMain, dmodMain);
  Application.CreateForm(TdmodRestCalc, dmodRestCalc);
  Application.Run;
end.
