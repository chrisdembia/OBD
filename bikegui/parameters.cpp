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
  
  wasMeijSelectedBefore = false;

  comboBox = new QComboBox(this);
  comboBox->addItem(tr("Gyrostat parameters"));
  comboBox->addItem(tr("Franke parameters"));
  comboBox->addItem(tr("Meijaard parameters"));

  layout = new QVBoxLayout(this);
  layout->addWidget(comboBox);

  initParamBox();
    
  connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT( paramBoxSlot(int)) );

//  bike->writeParameters(const_cast<char*>("whipple_workingparams.txt"));

}

void WhippleParameter::initParamBox()
{
  paramBox = new QGroupBox( tr("Set parameter values"), this);

  paramBox->setMinimumSize(200,600);
  paramBox->setMaximumSize(200,600);

  layout->addWidget(paramBox);
}

void WhippleParameter::paramBoxSlot(int index)
{
  delete paramBox;
  initParamBox();
  if (index == 0)
  {
  
  }
  else if (index == 1)
  { // Franke parameters
    QGridLayout * frankeParamLayout = new QGridLayout;
    frankeParamLayout->addWidget(new QLabel( tr("Not available. Bug the developers!") ),0,0);
    paramBox->setLayout(frankeParamLayout);
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
  QGridLayout *gyroParamLayout = new QGridLayout;
//  gyroParamLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

}

void WhippleParameter::drawMeijParamBox()
{
  // putting benchmark parameter widgets into a QGridLayout
  QGridLayout *meijParamLayout = new QGridLayout;
  meijParamLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
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
  connect(meijBenchParamsButton, SIGNAL(clicked()), this, SLOT(setBenchmarkParametersSlot()) );
 
  // convert Meijaard parameters to Gyrostat parameters
  QToolButton * meijToGyroButton = new QToolButton;
  meijToGyroButton->setText("Convert Meij. to Gyro params");

  // error message box
  QTextEdit * meijErrorText = new QTextEdit("Parameters are okay.");

 
  meijParamLayout->addWidget(meijLoadLabel,0,0);
  meijParamLayout->addWidget(meijLoadButton,0,1);
  meijParamLayout->addWidget(meijSaveButton,0,2);
  meijParamLayout->addWidget(meijFileLabel,1,0,1,3);
  meijParamLayout->addWidget(meijBenchParamsButton,2,0,1,3);
  meijParamLayout->addWidget(meijToGyroButton,3,0,1,3);
  meijParamLayout->addWidget(meijErrorText,4,0,3,3);  
  for (int i = 0; i < NmeijParams; i++)
  {
    meijParamLabels[i] = new QLabel( tr(meijParamStrings[i].c_str()) ); // dont need to save these
    meijParamLabels[i]->setToolTip( tr(meijParamToolTips[i].c_str() ) );

    meijParamEdits[i] = new QLineEdit;
    meijParamEdits[i]->setAlignment(Qt::AlignRight);
    meijParamEdits[i]->setValidator(new QDoubleValidator(-999.0, 999.0, 5, meijParamEdits[i]));

    meijParamLayout->addWidget(meijParamLabels[i],i+7,0);
    meijParamLayout->addWidget(meijParamEdits[i],i+7,1,1,2);
  }
  meijParamEdits[4]->setReadOnly(true); // forward velocity is not a real parameter
  // initialize parameters to the benchmark bicycle.
  if (!wasMeijSelectedBefore)
  {
    setBenchmarkParametersSlot();
    wasMeijSelectedBefore = true;
  }
  else
  {
    meijFileLabel->setText("<b>" + meijfdirname + "</b>");
    updateMeijParamEdits();
  }

  for (int i = 0; i < NmeijParams; i++)
  {
    connect(meijParamEdits[i], SIGNAL(editingFinished()),this,SLOT(meijAsteriskSlot()));
  }


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

  // from whippleutils.h
  convertParameters( gswp, mjwp);

    bool validparams = bike->setParameters( gswp);
  // set the parameters to be used by the Whipple bike object.
  try
  {  // NOTE: QT WILL NOT CATCH EXCEPTIONS FOR US, SO ALL EXCEPTION-THROWING CODE MUST BE IN MY OWN TRY..CATCH
    bool validparams = bike->setParameters( gswp);
  }
  catch (const char* errmsg)
  {
    std::cout << errmsg << std::endl;
  }
}

void WhippleParameter::meijAsteriskSlot()
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
  
  updateMeijParamEdits();
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

void WhippleParameter::setBenchmarkParametersSlot()
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
}

void WhippleParameter::gyroAsteriskSlot()
{
}

void WhippleParameter::gyroLoadSlot()
{
}

void WhippleParameter::gyroSaveSlot()
{
}

void WhippleParameter::gyroSaveAsSlot()
{
}


void WhippleParameter::defineMeijStrings()
{
  // MEIJAARD PARAMETERS
  meijParamStrings[0] = "w (m)";
  meijParamStrings[1] = "c (m)";
  meijParamStrings[2] = "lambda (rad)";
  meijParamStrings[3] = "g (m/s^2)";
  meijParamStrings[4] = "v (m/s)";
  meijParamStrings[5] = "t_{R} (m)";
  meijParamStrings[6] = "t_{F} (m)";
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
