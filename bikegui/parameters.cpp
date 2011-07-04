#include <string>
#include <QtGui>
#include "whipple.h"
#include "whippleutils.h"
#include "parameters.h"

WhippleParameter::WhippleParameter( Whipple *b, QWidget *parent)
  : QWidget(parent)
{

  // bring the bike pointer(s) from mainwindow over to parameters.
  bike = b;

  // struct from whipple.h, to hold the parambox values
  gswp = new WhippleParams; // gyrostat whipple parameters
  mjwp = new MJWhippleParams; // Meijaard whipple parameters
 
  wasGyroSelectedBefore = false; 
  wasMeijSelectedBefore = false;

  comboBox = new QComboBox(this);
  comboBox->addItem(tr("Gyrostat parameters"));
  comboBox->addItem(tr("Franke parameters"));
  comboBox->addItem(tr("Meijaard parameters"));

  layout = new QVBoxLayout(this);
  layout->addWidget(comboBox);

  initParamBox();
  drawGyroParamBox();
    
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT( paramBoxSlot(int)) );
}

void WhippleParameter::initParamBox()
{
  paramBox = new QGroupBox( tr("Set parameter values"), this);

  paramBox->setMinimumSize(200,600);
//  paramBox->setMaximumHeight(600);
  paramBox->resize(200,600);
  layout->addWidget(paramBox);
}

void WhippleParameter::paramBoxSlot(int index)
{
  delete paramBox;
  initParamBox();
  
  if (index == 0)
  {
    drawGyroParamBox();  
  }
  else if (index == 1)
  { // Franke parameters
    QGridLayout * frankeParamLayout = new QGridLayout;
    frankeParamLayout->addWidget(new QLabel( tr("Not available. Bug the developers!") ),0,0);
    paramBox->setLayout(frankeParamLayout);
//    drawGyroParamBox();
  }
  else if (index == 2)
  { // Meijaard parameters
    drawMeijParamBox();
  }
  // IMPLEMENT EVERYTHING ELSE.
  // manage deleting objects etc.
}

void WhippleParameter::drawGyroParamBox()
{
  // putting gyrostat parameter widgets into a QGridLayout
//  gyroParamLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  
  QGroupBox * gyroParamFileBox = new QGroupBox( tr("File management"),paramBox );
  QGroupBox * gyroParamModBox = new QGroupBox( tr("Modify parameters"),paramBox );
  gyroParamFileBox->setMaximumHeight(175);
  // putting benchmark parameter widgets into a QGridLayout
  QVBoxLayout * gyroParamLayout = new QVBoxLayout(paramBox);
  QGridLayout * gyroParamFileLayout = new QGridLayout(gyroParamFileBox);
  QGridLayout * gyroParamModLayout = new QGridLayout(gyroParamModBox);

  gyroParamFileBox->setLayout(gyroParamFileLayout);
  gyroParamModBox->setLayout(gyroParamModLayout);
  gyroParamLayout->addWidget(gyroParamFileBox);

  defineGyroStrings();
  
  // load parameters label
  QLabel * gyroLoadLabel = new QLabel("current file:");

  // load parameters button
  QToolButton * gyroLoadButton = new QToolButton;
  gyroLoadButton->setText("Load");
  connect(gyroLoadButton,SIGNAL(clicked()),this,SLOT(gyroLoadSlot()));
  
  // save parameters button
  QToolButton * gyroSaveButton = new QToolButton;
  gyroSaveButton->setText("Save");
  connect(gyroSaveButton,SIGNAL(clicked()),this,SLOT(gyroSaveSlot()));

  // line edit to show open file
  gyroFileLabel = new QLabel;
  gyroFileLabel->setTextFormat(Qt::RichText);
  // the value for the file label is set further down 
  
  // use benchmark parameters button
  QToolButton * gyroBenchParamsButton = new QToolButton;
  gyroBenchParamsButton->setText("Use benchmark parameters");
  connect(gyroBenchParamsButton, SIGNAL(clicked()), this, SLOT(setGyroBenchParametersSlot()) );
  
  // error message box
  gyroErrorText = new QTextEdit("Error messages will appear here.");

 
  gyroParamFileLayout->addWidget(gyroLoadLabel,0,0);
  gyroParamFileLayout->addWidget(gyroLoadButton,0,1);
  gyroParamFileLayout->addWidget(gyroSaveButton,0,2);
  gyroParamFileLayout->addWidget(gyroFileLabel,1,0,1,3);
  gyroParamFileLayout->addWidget(gyroBenchParamsButton,2,0,1,3);
  gyroParamFileLayout->addWidget(gyroErrorText,4,0,3,3);  
  
  for (int i = 0; i < NgyroParams; i++)
  {
    gyroParamLabels[i] = new QLabel( tr(gyroParamStrings[i].c_str()), paramBox ); // dont need to save these
    gyroParamLabels[i]->setToolTip( tr(gyroParamToolTips[i].c_str() ) );

    gyroParamEdits[i] = new QLineEdit( paramBox);
    gyroParamEdits[i]->setAlignment(Qt::AlignRight);
    gyroParamEdits[i]->setValidator(new QDoubleValidator(-999.0, 999.0, 5, gyroParamEdits[i]));

    gyroParamModLayout->addWidget(gyroParamLabels[i],i,0);
    gyroParamModLayout->addWidget(gyroParamEdits[i],i,1,1,2);
  }
  
// initialize parameters to the benchmark bicycle.
  if (!wasGyroSelectedBefore)
  {
    setGyroBenchParametersSlot();
    wasGyroSelectedBefore = true;
  }
  else
  {
    gyroFileLabel->setText("<b>" + gyrofdirname + "</b>");

    updateGyroParamEdits();
  }

  for (int i = 0; i < NgyroParams; i++)
  {
    connect(gyroParamEdits[i], SIGNAL(textEdited(const QString&)),this,SLOT(gyroAsteriskSlot(const QString&)));
  }

  QScrollArea * gyroParamModScroll = new QScrollArea;
  gyroParamModScroll->setWidget(gyroParamModBox);
  
  gyroParamLayout->addWidget(gyroParamModScroll);  

  paramBox->setLayout(gyroParamLayout);
}

void WhippleParameter::drawMeijParamBox()
{
  QGroupBox * meijParamFileBox = new QGroupBox( tr("File management"),paramBox );
  QGroupBox * meijParamModBox = new QGroupBox( tr("Modify parameters"),paramBox );
  meijParamFileBox->setMaximumHeight(175);
  // putting benchmark parameter widgets into a QGridLayout
  QVBoxLayout * meijParamLayout = new QVBoxLayout(paramBox);
  QGridLayout * meijParamFileLayout = new QGridLayout(meijParamFileBox);
  QGridLayout * meijParamModLayout = new QGridLayout(meijParamModBox);

  meijParamFileBox->setLayout(meijParamFileLayout);
  meijParamModBox->setLayout(meijParamModLayout);
  meijParamLayout->addWidget(meijParamFileBox);
  

//  meijParamLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  // DO I NEED THE ABOVE?
 
  defineMeijStrings();
  
  // load parameters label
  QLabel * meijLoadLabel = new QLabel("current file:");

  // load parameters button
  QToolButton * meijLoadButton = new QToolButton;
  meijLoadButton->setText("Load");
  connect(meijLoadButton,SIGNAL(clicked()),this,SLOT(meijLoadSlot()));
  
  // save parameters button
  QToolButton * meijSaveButton = new QToolButton;
  meijSaveButton->setText("Save");
  connect(meijSaveButton,SIGNAL(clicked()),this,SLOT(meijSaveSlot()));

  // line edit to show open file
  meijFileLabel = new QLabel;
  meijFileLabel->setTextFormat(Qt::RichText);
  // the value for the file label is set further down 
  
  // use benchmark parameters button
  QToolButton * meijBenchParamsButton = new QToolButton;
  meijBenchParamsButton->setText("Use benchmark parameters");
  connect(meijBenchParamsButton, SIGNAL(clicked()), this, SLOT(setMeijBenchParametersSlot()) );
 
  // convert Meijaard parameters to Gyrostat parameters
  QToolButton * meijToGyroButton = new QToolButton;
  meijToGyroButton->setText("Convert Meij. to Gyro params");

  // error message box
  meijErrorText = new QTextEdit("Parameters are okay.");

 
  meijParamFileLayout->addWidget(meijLoadLabel,0,0);
  meijParamFileLayout->addWidget(meijLoadButton,0,1);
  meijParamFileLayout->addWidget(meijSaveButton,0,2);
  meijParamFileLayout->addWidget(meijFileLabel,1,0,1,3);
  meijParamFileLayout->addWidget(meijBenchParamsButton,2,0,1,3);
  meijParamFileLayout->addWidget(meijToGyroButton,3,0,1,3);
  meijParamFileLayout->addWidget(meijErrorText,4,0,3,3);  
  for (int i = 0; i < NmeijParams; i++)
  {
    meijParamLabels[i] = new QLabel( tr(meijParamStrings[i].c_str()), paramBox ); // dont need to save these
    meijParamLabels[i]->setToolTip( tr(meijParamToolTips[i].c_str() ) );

    meijParamEdits[i] = new QLineEdit( paramBox);
    meijParamEdits[i]->setAlignment(Qt::AlignRight);
    meijParamEdits[i]->setValidator(new QDoubleValidator(-999.0, 999.0, 5, meijParamEdits[i]));

    meijParamModLayout->addWidget(meijParamLabels[i],i,0);
    meijParamModLayout->addWidget(meijParamEdits[i],i,1,1,2);
  }
  meijParamEdits[4]->setReadOnly(true); // forward velocity is not a real parameter
  // initialize parameters to the benchmark bicycle.
  if (!wasMeijSelectedBefore)
  {
    setMeijBenchParametersSlot();
    wasMeijSelectedBefore = true;
  }
  else
  {
    meijFileLabel->setText("<b>" + meijfdirname + "</b>");
    updateMeijParamEdits();
  }

  for (int i = 0; i < NmeijParams; i++)
  {
    connect(meijParamEdits[i], SIGNAL(textEdited(const QString&)),this,SLOT(meijAsteriskSlot(const QString&)));
  }

  
  QScrollArea * meijParamModScroll = new QScrollArea;
// meijParamModScroll->setBackgroundRole(QPalette::Dark);
  meijParamModScroll->setWidget(meijParamModBox);
  
  meijParamLayout->addWidget(meijParamModScroll);  
  paramBox->setLayout(meijParamLayout);
}

void WhippleParameter::updateMeijParamEdits(void)
{
  // BENCHMARK MODEL PARAMETERS
  meijParamValues[0] = mjwp->w;
  meijParamValues[1] = mjwp->c;
  meijParamValues[2] = mjwp->lambda;
  meijParamValues[3] = mjwp->g;
  meijParamValues[4] = 1337;
  meijParamValues[5] = mjwp->rrt;
  meijParamValues[6] = mjwp->rft;
  meijParamValues[7] = mjwp->rr;
  meijParamValues[8] = mjwp->mr;
  meijParamValues[9] = mjwp->IRxx;
  meijParamValues[10] = mjwp->IRyy;
  meijParamValues[11] = mjwp->xb;
  meijParamValues[12] = mjwp->zb;
  meijParamValues[13] = mjwp->mb;
  meijParamValues[14] = mjwp->IBxx;
  meijParamValues[15] = mjwp->IByy;
  meijParamValues[16] = mjwp->IBzz;
  meijParamValues[17] = mjwp->IBxz;
  meijParamValues[18] = mjwp->xh;
  meijParamValues[19] = mjwp->zh;
  meijParamValues[20] = mjwp->mh;
  meijParamValues[21] = mjwp->IHxx;
  meijParamValues[22] = mjwp->IHyy;
  meijParamValues[23] = mjwp->IHzz;
  meijParamValues[24] = mjwp->IHxz;
  meijParamValues[25] = mjwp->rf;
  meijParamValues[26] = mjwp->mf;
  meijParamValues[27] = mjwp->IFxx;
  meijParamValues[28] = mjwp->IFyy;
  // add a horizontal rule
  for (int k = 0; k < NmeijParams; k++)
  {
    meijParamEdits[k]->setText( QString("%1").arg(meijParamValues[k]) );
  }

  sendMeijParamToBike();
}

void WhippleParameter::meijAsteriskSlot(const QString& ss)
{
  // add an asterisk to the dialog box
  if ( !meijFileLabel->text().endsWith("*</b>") )
  {
    QString qstr = meijFileLabel->text();
    meijFileLabel->setText( qstr.insert( qstr.lastIndexOf("<"), "*") );
  }

  // revalidate and save the parameters  
  mjwp->w = meijParamEdits[0]->text().toDouble();
  mjwp->c = meijParamEdits[1]->text().toDouble();
  mjwp->lambda = meijParamEdits[2]->text().toDouble();
  mjwp->g = meijParamEdits[3]->text().toDouble();
  mjwp->rrt = meijParamEdits[5]->text().toDouble();
  mjwp->rft = meijParamEdits[6]->text().toDouble();
  mjwp->rr = meijParamEdits[7]->text().toDouble();
  mjwp->mr = meijParamEdits[8]->text().toDouble();
  mjwp->IRxx = meijParamEdits[9]->text().toDouble();
  mjwp->IRyy = meijParamEdits[10]->text().toDouble();
  mjwp->xb = meijParamEdits[11]->text().toDouble();
  mjwp->zb = meijParamEdits[12]->text().toDouble();
  mjwp->mb = meijParamEdits[13]->text().toDouble();
  mjwp->IBxx = meijParamEdits[14]->text().toDouble();
  mjwp->IByy = meijParamEdits[15]->text().toDouble();
  mjwp->IBzz = meijParamEdits[16]->text().toDouble();
  mjwp->IBxz = meijParamEdits[17]->text().toDouble();
  mjwp->xh = meijParamEdits[18]->text().toDouble();
  mjwp->zh = meijParamEdits[19]->text().toDouble();
  mjwp->mh = meijParamEdits[20]->text().toDouble();
  mjwp->IHxx = meijParamEdits[21]->text().toDouble();
  mjwp->IHyy = meijParamEdits[22]->text().toDouble();
  mjwp->IHzz = meijParamEdits[23]->text().toDouble();
  mjwp->IHxz = meijParamEdits[24]->text().toDouble();
  mjwp->rf = meijParamEdits[25]->text().toDouble();
  mjwp->mf = meijParamEdits[26]->text().toDouble();
  mjwp->IFxx = meijParamEdits[27]->text().toDouble();
  mjwp->IFyy = meijParamEdits[28]->text().toDouble();
  
  sendMeijParamToBike();
}

void WhippleParameter::sendMeijParamToBike()
{

  bool validparameters = false;
  meijErrorText->setTextColor( Qt::black );
  meijErrorText->setText( tr("Error messages appear here.") );
  // set the parameters to be used by the Whipple bike object.
  try
  {  // NOTE: QT WILL NOT CATCH EXCEPTIONS FOR US, SO ALL EXCEPTION-THROWING CODE MUST BE IN MY OWN TRY..CATCH

   // MAY TRY TO IMPLEMENT TRYCATCH INTO CONVERTPARAMETERS INSTEAD. NOTE THAT THERE ARE ERRORS BEING CAUGHT BY GYROSTAT SET THAT ARE NOT BEING CAUGHT BY WHIPPLE SET
    validparameters = validateMJParameters( mjwp, true);
  }
  catch (const char* errmsg)
  {
    meijErrorText->setTextColor( Qt::red );
    meijErrorText->setText( tr(errmsg) );
  }
  catch (...)
  {
    std::cerr << "Caught an unknown exception in WhippleParameter::sendMeijParamToBike(). Tried to run bike->validateMJParameters( MJWhippleParams*, bool ), which should only throw const char *'s." << std::endl;
  }

  if (validparameters)
  { // from whippleutils.h
    WhippleParams *gswptemp = new WhippleParams;
    convertParameters( gswptemp, mjwp);
    try
    {
      bike->setParameters( gswptemp, true);
    }
    catch (...)
    {
      meijErrorText->setTextColor( Qt::red );
      meijErrorText->setText( tr("Caught an error after internally converting Meijaard parameters to Gyrostat parameters.") );
    }
    delete gswptemp;
  }

}

void WhippleParameter::meijLoadSlot()
{
  QString fdirname = QFileDialog::getOpenFileName(this, tr("Load file"), QDir::currentPath() );
  if (!fdirname.isEmpty())
  {
    meijfdirname = fdirname;
    readMJWhippleParams(mjwp, meijfdirname.toStdString().c_str() );
    updateMeijParamEdits(); 
    meijFileLabel->setText("<b>" + meijfdirname + "</b>");
  }
}

void WhippleParameter::meijSaveSlot()
{
  QString fdirname = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("Text file (*, *.txt, *.dat,...);;Any file (*)") );
  // from whippleutils.h
  if (!fdirname.isEmpty())
  {
    meijfdirname = fdirname;
    writeMJWhippleParams(mjwp, meijfdirname.toStdString().c_str() );
    meijFileLabel->setText("<b>" + meijfdirname + "</b>");
  }
}

void WhippleParameter::meijSaveAsSlot()
{
}

void WhippleParameter::setMeijBenchParametersSlot()
{
  // in whippleutils.h
  setBenchmarkParameters(mjwp);

  // update line edits
  updateMeijParamEdits();
  
  // set the displayed file name appropriately (the data is built in)
  meijfdirname = QString("benchmark");
  meijFileLabel->setText("<b>" + meijfdirname + "</b>");
}

void WhippleParameter::updateGyroParamEdits(void)
{
  gyroParamValues[0] = gswp->rr;
  gyroParamValues[1] = gswp->rrt;
  gyroParamValues[2] = gswp->rf;
  gyroParamValues[3] = gswp->rft;
  gyroParamValues[4] = gswp->lr;
  gyroParamValues[5] = gswp->ls;
  gyroParamValues[6] = gswp->lf;
  gyroParamValues[7] = gswp->mr;
  gyroParamValues[8] = gswp->mf;
  gyroParamValues[9] = gswp->ICyy;
  gyroParamValues[10] = gswp->IDxx;
  gyroParamValues[11] = gswp->IDyy;
  gyroParamValues[12] = gswp->IDzz;
  gyroParamValues[13] = gswp->IDxz;
  gyroParamValues[14] = gswp->IExx;
  gyroParamValues[15] = gswp->IEyy;
  gyroParamValues[16] = gswp->IEzz;
  gyroParamValues[17] = gswp->IExz;
  gyroParamValues[18] = gswp->IFyy;
  gyroParamValues[19] = gswp->lrx;
  gyroParamValues[20] = gswp->lrz;
  gyroParamValues[21] = gswp->lfx;
  gyroParamValues[22] = gswp->lfz;
  gyroParamValues[23] = gswp->g;
  
  for (int k = 0; k < NgyroParams; k++)
  {
    gyroParamEdits[k]->setText( QString("%1").arg(gyroParamValues[k]) );
  }
  
  sendGyroParamToBike();
}

void WhippleParameter::gyroAsteriskSlot(const QString&)
{
  // add an asterisk to the dialog box
  if ( !gyroFileLabel->text().endsWith("*</b>") )
  {
    QString qstr = gyroFileLabel->text();
    gyroFileLabel->setText( qstr.insert( qstr.lastIndexOf("<"), "*") );
  }

  // revalidate and save the parameters  
  gswp->rr =    gyroParamEdits[0]->text().toDouble();
  gswp->rrt =   gyroParamEdits[1]->text().toDouble();   
  gswp->rf =    gyroParamEdits[2]->text().toDouble(); 
  gswp->rft =   gyroParamEdits[3]->text().toDouble(); 
  gswp->lr =    gyroParamEdits[4]->text().toDouble(); 
  gswp->ls =    gyroParamEdits[5]->text().toDouble(); 
  gswp->lf =    gyroParamEdits[6]->text().toDouble(); 
  gswp->mr =    gyroParamEdits[7]->text().toDouble(); 
  gswp->mf =    gyroParamEdits[8]->text().toDouble(); 
  gswp->ICyy =  gyroParamEdits[9]->text().toDouble(); 
  gswp->IDxx =  gyroParamEdits[10]->text().toDouble();
  gswp->IDyy =  gyroParamEdits[11]->text().toDouble();
  gswp->IDzz =  gyroParamEdits[12]->text().toDouble();
  gswp->IDxz =  gyroParamEdits[13]->text().toDouble();
  gswp->IExx =  gyroParamEdits[14]->text().toDouble();
  gswp->IEyy =  gyroParamEdits[15]->text().toDouble();
  gswp->IEzz =  gyroParamEdits[16]->text().toDouble();
  gswp->IExz =  gyroParamEdits[17]->text().toDouble();
  gswp->IFyy =  gyroParamEdits[18]->text().toDouble();
  gswp->lrx =   gyroParamEdits[19]->text().toDouble();
  gswp->lrz =   gyroParamEdits[20]->text().toDouble();
  gswp->lfx =   gyroParamEdits[21]->text().toDouble();
  gswp->lfz =   gyroParamEdits[22]->text().toDouble();
  gswp->g =     gyroParamEdits[23]->text().toDouble();

  sendGyroParamToBike();  
}

void WhippleParameter::sendGyroParamToBike()
{

  // set the parameters to be used by the Whipple bike object.
  gyroErrorText->setTextColor( Qt::black );
  gyroErrorText->setText( tr("Error messages appear here.") );
  try
  {  // NOTE: QT WILL NOT CATCH EXCEPTIONS FOR US, SO ALL EXCEPTION-THROWING CODE MUST BE IN MY OWN TRY..CATCH
    bike->setParameters( gswp, true);
  }
  catch (const char * errmsg)
  {
    gyroErrorText->setTextColor( Qt::red );
    gyroErrorText->setText( tr(errmsg) );
  }
  catch (...)
  {
    std::cerr << "Caught an unknown exception in WhippleParameter::sendGyroParamToBike(). Tried to run bike->setParameters( WhippleParams*, bool ), which should only throw const char *'s." << std::endl;
  }
}

void WhippleParameter::gyroLoadSlot()
{
  QString fdirname = QFileDialog::getOpenFileName(this, tr("Load file"), QDir::currentPath() );
  if (!fdirname.isEmpty())
  {
    gyrofdirname = fdirname;
    // from whippleutils.h
std::cout << gswp->rr << std::endl;
    readWhippleParams(gswp, gyrofdirname.toStdString().c_str() );
std::cout << gswp->rr << std::endl;
    updateGyroParamEdits(); 
    gyroFileLabel->setText("<b>" + gyrofdirname + "</b>");
  }
}

void WhippleParameter::gyroSaveSlot()
{
  QString fdirname = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("Text file (*, *.txt, *.dat,...);;Any file (*)") );
  // from whippleutils.h
  if (!fdirname.isEmpty())
  {
    gyrofdirname = fdirname;
    bike->writeParameters( gyrofdirname.toStdString().c_str() );
    gyroFileLabel->setText("<b>" + fdirname + "</b>");
  }
}

void WhippleParameter::gyroSaveAsSlot()
{
}

void WhippleParameter::setGyroBenchParametersSlot()
{
  // in whippleutils.h
  MJWhippleParams *mjwptemp = new MJWhippleParams;
  setBenchmarkParameters( mjwptemp);
  convertParameters( gswp, mjwptemp);

  updateGyroParamEdits();

  gyrofdirname = QString("benchmark");
  gyroFileLabel->setText("<b>" + gyrofdirname + "</b>");
  
} 

void WhippleParameter::defineMeijStrings()
{
  // MEIJAARD PARAMETERS
  meijParamStrings[0] = "w (m)";
  meijParamStrings[1] = "c (m)";
  meijParamStrings[2] = "lambda (rad)";
  meijParamStrings[3] = "g (m/s^2)";
  meijParamStrings[4] = "v (m/s)";
  meijParamStrings[5] = "r_{Rt} (m)";
  meijParamStrings[6] = "r_{Ft} (m)";
  meijParamStrings[7] = "r_R (m)";
  meijParamStrings[8] = "m_R (kg)";
  meijParamStrings[9] = "I_{Rxx} (kg m^2)";
  meijParamStrings[10] = "I_{Ryy} (kg m^2)";
  meijParamStrings[11] = "x_B (m)";
  meijParamStrings[12] = "z_B (m)";
  meijParamStrings[13] = "m_B (kg)";
  meijParamStrings[14] = "I_{Bxx} (kg m^2)";
  meijParamStrings[15] = "I_{Byy} (kg m^2)";
  meijParamStrings[16] = "I_{Bzz} (kg m^2)";
  meijParamStrings[17] = "I_{Bxz} (kg m^2)";
  meijParamStrings[18] = "x_H (m)";
  meijParamStrings[19] = "z_H (m)";
  meijParamStrings[20] = "m_H (m)";
  meijParamStrings[21] = "I_{Hxx} (kg m^2)";
  meijParamStrings[22] = "I_{Hyy} (kg m^2)";
  meijParamStrings[23] = "I_{Hzz} (kg m^2)";
  meijParamStrings[24] = "I_{Hxz} (kg m^2)";
  meijParamStrings[25] = "r_F (m)";
  meijParamStrings[26] = "m_F (kg)";
  meijParamStrings[27] = "I_{Fxx} (kg m^2)";
  meijParamStrings[28] = "I_{Fyy} (kg m^2)";

  meijParamToolTips[0] = "Wheel base: distance between contact patches of the front and rear wheels.";
  for (int i = 1; i < NmeijParams; i++ )
  {
  meijParamToolTips[i] = "EDIT";
  }
} 

void WhippleParameter::defineGyroStrings()
{
  // GYROSTAT PARAMETERS
  gyroParamStrings[0] = "r_R (m)";
  gyroParamStrings[1] = "r_{Rt} (m)";
  gyroParamStrings[2] = "r_F (m)";
  gyroParamStrings[3] = "r_{Ft} (m)";
  gyroParamStrings[4] = "l_{R} (m)";
  gyroParamStrings[5] = "l_{S} (m)";
  gyroParamStrings[6] = "l_{F} (m)";
  gyroParamStrings[7] = "m_{R} (kg)";
  gyroParamStrings[8] = "m_{F} (kg)";
  gyroParamStrings[9] = "I_{Cyy} (kg-m^2)";
  gyroParamStrings[10] = "I_{Dxx} (kg-m^2)";
  gyroParamStrings[11] = "I_{Dyy} (kg-m^2)";
  gyroParamStrings[12] = "I_{Dzz} (kg-m^2)";
  gyroParamStrings[13] = "I_{Dxz} (kg-m^2)";
  gyroParamStrings[14] = "I_{Exx} (kg-m^2)";
  gyroParamStrings[15] = "I_{Eyy} (kg-m^2)";
  gyroParamStrings[16] = "I_{Ezz} (kg-m^2)";
  gyroParamStrings[17] = "I_{Exz} (kg-m^2)";
  gyroParamStrings[18] = "I_{Fyy} (kg-m^2)";
  gyroParamStrings[19] = "l_{Rx} (m)";
  gyroParamStrings[20] = "l_{Rz} (m)";
  gyroParamStrings[21] = "l_{Fx} (m)";
  gyroParamStrings[22] = "l_{Fz} (m)";
  gyroParamStrings[23] = "g (m/s^2)";

  gyroParamToolTips[0] = "rear wheel radius";
  for (int i = 1; i < NgyroParams; i++)
  {
    gyroParamToolTips[i] = "EDIT";
  }
} 




































 
