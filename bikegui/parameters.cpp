#include <string>
#include <QtGui>
#include "parameters.h"

using namespace std;

WhippleParameter::WhippleParameter(QWidget *parent)
  : QWidget(parent)
{
  comboBox = new QComboBox(this);
  comboBox->addItem(tr("Gyrostat parameters"));
  comboBox->addItem(tr("Franke parameters"));
  comboBox->addItem(tr("Benchmark parameters"));

  paramBox = new QGroupBox( tr("Set parameter values"), this);
  
  QScrollArea * paramScrollArea = new QScrollArea;
  paramScrollArea->setBackgroundRole(QPalette::Dark);
//  paramScrollArea->setWidget(paramBox);
  
  // putting benchmark parameter widgets into a QGridLayout
  QGridLayout *benchParamLayout = new QGridLayout;
  benchParamLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  
  const int NbenchParams = 29;
  QLabel * benchParamLabels[NbenchParams];
  QLineEdit * benchParamEdits[NbenchParams];
  string benchParamStrings[NbenchParams];
  string benchParamValues[NbenchParams];
  
  benchParamStrings[0] = "w (m)"; benchParamValues[0] = "1.02";
  benchParamStrings[1] = "c (m)"; benchParamValues[1] = "0.08";
  benchParamStrings[2] = "lambda (rad)"; benchParamValues[2] = "pi/10";
  benchParamStrings[3] = "g (m/s^2)"; benchParamValues[3] = "9.81";
  benchParamStrings[4] = "v (m/s)"; benchParamValues[4] = "idk";
  benchParamStrings[5] = "r_R (m)"; benchParamValues[5] = "0.3";
  benchParamStrings[6] = "t_{R} (m)"; benchParamValues[6] = "0.0";
  benchParamStrings[7] = "t_{F} (m)"; benchParamValues[7] = "0.0";
  benchParamStrings[8] = "m_R (kg)"; benchParamValues[8] = "2.0";
  benchParamStrings[9] = "I_{Rxx} (kg m^2)"; benchParamValues[9] = "0.0603";
  benchParamStrings[10] = "I_{Ryy} (kg m^2)"; benchParamValues[10] = "0.12";
  benchParamStrings[11] = "x_B (m)"; benchParamValues[11] = "0.3";
  benchParamStrings[12] = "z_B (m)"; benchParamValues[12] = "0.9";
  benchParamStrings[13] = "m_B (kg)"; benchParamValues[13] = "85.0";
  benchParamStrings[14] = "I_{Bxx} (kg m^2)"; benchParamValues[14] = "9.2";
  benchParamStrings[15] = "I_{Byy} (kg m^2)"; benchParamValues[15] = "11.0";
  benchParamStrings[16] = "I_{Bzz} (kg m^2)"; benchParamValues[16] = "2.8";
  benchParamStrings[17] = "I_{Bxz} (kg m^2)"; benchParamValues[17] = "2.4";
  benchParamStrings[18] = "x_H (m)"; benchParamValues[18] = "0.9";
  benchParamStrings[19] = "z_H (m)"; benchParamValues[19] = "0.7";
  benchParamStrings[20] = "m_H (m)"; benchParamValues[20] = "4.0";
  benchParamStrings[21] = "I_{Hxx} (kg m^2)"; benchParamValues[21] = "0.05892";
  benchParamStrings[22] = "I_{Hyy} (kg m^2)"; benchParamValues[22] = "0.06";
  benchParamStrings[23] = "I_{Hzz} (kg m^2)"; benchParamValues[23] = "0.00708";
  benchParamStrings[24] = "I_{Hxz} (kg m^2)"; benchParamValues[24] = "0.00756";
  benchParamStrings[25] = "r_F (m)"; benchParamValues[25] = "0.35";
  benchParamStrings[26] = "m_F (kg)"; benchParamValues[26] = "3.0";
  benchParamStrings[27] = "I_{Fxx} (kg m^2)"; benchParamValues[27] = "0.1405";
  benchParamStrings[28] = "I_{Fyy} (kg m^2)"; benchParamValues[28] = "0.28";
  
  QLabel * benchLoadLabel = new QLabel("Load parameter file");
  QToolButton * benchLoadButton = new QToolButton;
  benchLoadButton->setText("Load");
  QToolButton * benchSaveButton = new QToolButton;
  benchSaveButton->setText("Save");
  QLineEdit * benchLoadEdit = new QLineEdit("put dialogbox here");

  benchParamLayout->addWidget(benchLoadLabel,0,0);
  benchParamLayout->addWidget(benchLoadButton,0,1);
  benchParamLayout->addWidget(benchSaveButton,0,2);
  benchParamLayout->addWidget(benchLoadEdit,1,0,1,3);
  // add a horizontal rule
  for (int i = 0; i < NbenchParams; i++)
  {
  	benchParamLabels[i] = new QLabel( tr(benchParamStrings[i].c_str()) );
  	benchParamEdits[i] = new QLineEdit( tr(benchParamValues[i].c_str()) );
	benchParamEdits[i]->setAlignment(Qt::AlignRight);
	
	benchParamLayout->addWidget(benchParamLabels[i],i+2,0);
	benchParamLayout->addWidget(benchParamEdits[i],i+2,1,1,2);
  }
  
  paramBox->setLayout(benchParamLayout);
       
  layout = new QVBoxLayout(this);
  layout->addWidget(comboBox);
  layout->addWidget(paramBox);
}

/*
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

	mainLayout->addWidget(display,0,0,1,6);
	mainLayout->addWidget(backspaceButton,1,0,1,2);
	mainLayout->addWidget(clearButton,1,2,1,2);
	mainLayout->addWidget(clearAllButton,1,4,1,2);
	
	 mainLayout->addWidget(clearMemoryButton, 2, 0);
     mainLayout->addWidget(readMemoryButton, 3, 0);
     mainLayout->addWidget(setMemoryButton, 4, 0);
     mainLayout->addWidget(addToMemoryButton, 5, 0);
     
     void MainWindow::createActions(void)
{
  // New action
  newAction = new QAction(tr("&New"), this);
  newAction->setShortcut(QKeySequence::New);
  newAction->setStatusTip(tr("Create a new set of bicycle parameters"));
  // connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

  // Open action
  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip(tr("Open a set of bicycle parameters"));
  // connect(openAction, SIGNAL(triggered()), this, SLOT(about()));

	display = new QLineEdit("0");
	display->setReadOnly(true);
	display->setAlignment(Qt::AlignRight);
	display->setMaxLength(15);

*/

/*
  // creating benchmark parameter label and lineEdit widgets
  QLabel * wheelbaseLabel = new QLabel( tr("w (m)") );
  QLineEdit * wheelbaseEdit = new QLineEdit("1.02");
  benchParamLayout->addWidget(wheelbaseLabel,0,0);
  benchParamLayout->addWidget(wheelbaseEdit,0,1);
    
  QLabel * trailLabel = new QLabel( tr("c (m)") );
  QLineEdit * trailEdit = new QLineEdit("0.08");
  benchParamLayout->addWidget(trailLabel,1,0);
  benchParamLayout->addWidget(trailEdit,1,1);
  
  QLabel * steeraxistiltLabel = new QLabel( tr("lambda (rad)") );
  QLineEdit * steeraxistiltEdit = new QLineEdit("pi/10");
  benchParamLayout->addWidget(steeraxistiltLabel,2,0);
  benchParamLayout->addWidget(steeraxistiltEdit,2,1);
  
  QLabel * accelgravityLabel = new QLabel( tr("g (m/s^2)") );
  QLineEdit * accelgravityEdit = new QLineEdit("9.81");
  benchParamLayout->addWidget(accelgravityLabel,3,0);
  benchParamLayout->addWidget(accelgravityEdit,3,1);
  
  QLabel * forwardspeedLabel = new QLabel( tr("v (m/s)") );
  QLineEdit * forwardspeedEdit = new QLineEdit("idk");
  benchParamLayout->addWidget(forwardspeedLabel,4,0);
  benchParamLayout->addWidget(forwardspeedEdit,4,1);
  
  QLabel * radiusRLabel = new QLabel( tr("r_R (m)") );
  QLineEdit * radiusREdit = new QLineEdit("0.3");
  benchParamLayout->addWidget(radiusRLabel,5,0);
  benchParamLayout->addWidget(radiusREdit,5,1);
  
  QLabel * thickRLabel = new QLabel( tr("t_{R} (m)") );
  QLineEdit * thickREdit = new QLineEdit("0.0");
  benchParamLayout->addWidget(thickRLabel,6,0);
  benchParamLayout->addWidget(thickREdit,6,1);
  
  QLabel * thickFLabel = new QLabel( tr("t_{F} (m)") );
  QLineEdit * thickFEdit = new QLineEdit("0.0");
  benchParamLayout->addWidget(thickFLabel,7,0);
  benchParamLayout->addWidget(thickFEdit,7,1);
  
  QLabel * massRLabel = new QLabel( tr("m_R (kg)") );
  QLineEdit * massREdit = new QLineEdit("2.0");
  benchParamLayout->addWidget(massRLabel,8,0);
  benchParamLayout->addWidget(massREdit,8,1);
  
  QLabel * inertiaRxxLabel = new QLabel( tr("I_{Rxx} (kg m^2)") );
  QLineEdit * inertiaRxxEdit = new QLineEdit("0.0603");
  benchParamLayout->addWidget(inertiaRxxLabel,9,0);
  benchParamLayout->addWidget(inertiaRxxEdit,9,1);
  
  QLabel * inertiaRyyLabel = new QLabel( tr("I_{Ryy} (kg m^2)") );
  QLineEdit * inertiaRyyEdit = new QLineEdit("0.12");
  benchParamLayout->addWidget(inertiaRyyLabel,10,0);
  benchParamLayout->addWidget(inertiaRyyEdit,10,1);
  
  QLabel * posBxLabel = new QLabel( tr("x_B (m)") );
  QLineEdit * posBxEdit = new QLineEdit("0.3");
  benchParamLayout->addWidget(posBxLabel,11,0);
  benchParamLayout->addWidget(posBxEdit,11,1);
  
  QLabel * posBzLabel = new QLabel( tr("z_B (m)") );
  QLineEdit * posBzEdit = new QLineEdit("0.9");
  benchParamLayout->addWidget(posBzLabel,12,0);
  benchParamLayout->addWidget(posBzEdit,12,1);
  
  QLabel * massBLabel = new QLabel( tr("m_B (kg)") );
  QLineEdit * massBEdit = new QLineEdit("85.0");
  benchParamLayout->addWidget(massBLabel,13,0);
  benchParamLayout->addWidget(massBEdit,13,1);
  
  QLabel * inertiaBxxLabel = new QLabel( tr("I_{Bxx} (kg m^2)") );
  QLineEdit * inertiaBxxEdit = new QLineEdit("9.2");
  benchParamLayout->addWidget(inertiaBxxLabel,14,0);
  benchParamLayout->addWidget(inertiaBxxEdit,14,1);
  
  QLabel * inertiaByyLabel = new QLabel( tr("I_{Byy} (kg m^2)") );
  QLineEdit * inertiaByyEdit = new QLineEdit("11.0");
  benchParamLayout->addWidget(inertiaByyLabel,15,0);
  benchParamLayout->addWidget(inertiaByyEdit,15,1);
  
  QLabel * inertiaBzzLabel = new QLabel( tr("I_{Bzz} (kg m^2)") );
  QLineEdit * inertiaBzzEdit = new QLineEdit("2.8");
  benchParamLayout->addWidget(inertiaBzzLabel,16,0);
  benchParamLayout->addWidget(inertiaBzzEdit,16,1);
  
  QLabel * inertiaBxzLabel = new QLabel( tr("I_{Bxz} (kg m^2)") );
  QLineEdit * inertiaBxzEdit = new QLineEdit("2.4");
  benchParamLayout->addWidget(inertiaBxzLabel,17,0);
  benchParamLayout->addWidget(inertiaBxzEdit,17,1);
  
  QLabel * posHxLabel = new QLabel( tr("x_H (m)") );
  QLineEdit * posHxEdit = new QLineEdit("0.9");
  benchParamLayout->addWidget(posHxLabel,18,0);
  benchParamLayout->addWidget(posHxEdit,18,1);
  
  QLabel * posHzLabel = new QLabel( tr("z_H (m)") );
  QLineEdit * posHzEdit = new QLineEdit("0.7");
  benchParamLayout->addWidget(posHzLabel,19,0);
  benchParamLayout->addWidget(posHzEdit,19,1);
  
  QLabel * massHLabel = new QLabel( tr("m_H (m)") );
  QLineEdit * massHEdit = new QLineEdit("4.0");
  benchParamLayout->addWidget(massHLabel,20,0);
  benchParamLayout->addWidget(massHEdit,20,1);
  
  QLabel * inertiaHxxLabel = new QLabel( tr("I_{Hxx} (kg m^2)") );
  QLineEdit * inertiaHxxEdit = new QLineEdit("0.05892");
  benchParamLayout->addWidget(inertiaHxxLabel,21,0);
  benchParamLayout->addWidget(inertiaHxxEdit,21,1);
  
  QLabel * inertiaHyyLabel = new QLabel( tr("I_{Hyy} (kg m^2)") );
  QLineEdit * inertiaHyyEdit = new QLineEdit("0.06");
  benchParamLayout->addWidget(inertiaHyyLabel,22,0);
  benchParamLayout->addWidget(inertiaHyyEdit,22,1);
  
  QLabel * inertiaHzzLabel = new QLabel( tr("I_{Hzz} (kg m^2)") );
  QLineEdit * inertiaHzzEdit = new QLineEdit("0.00708");
  benchParamLayout->addWidget(inertiaHzzLabel,23,0);
  benchParamLayout->addWidget(inertiaHzzEdit,23,1);
  
  QLabel * inertiaHxzLabel = new QLabel( tr("I_{Hxz} (kg m^2)") );
  QLineEdit * inertiaHxzEdit = new QLineEdit("0.00756");
  benchParamLayout->addWidget(inertiaHxzLabel,24,0);
  benchParamLayout->addWidget(inertiaHxzEdit,24,1);
  
  QLabel * radiusFLabel = new QLabel( tr("r_F (m)") );
  QLineEdit * radiusFEdit = new QLineEdit("0.35");
  benchParamLayout->addWidget(radiusFLabel,25,0);
  benchParamLayout->addWidget(radiusFEdit,25,1);
  
  QLabel * massFLabel = new QLabel( tr("m_F (kg)") );
  QLineEdit * massFEdit = new QLineEdit("3.0");
  benchParamLayout->addWidget(massFLabel,26,0);
  benchParamLayout->addWidget(massFEdit,26,1);
  
  QLabel * inertiaFxxLabel = new QLabel( tr("I_{Fxx} (kg m^2)") );
  QLineEdit * inertiaFxxEdit = new QLineEdit("0.1405");
  benchParamLayout->addWidget(inertiaFxxLabel,27,0);
  benchParamLayout->addWidget(inertiaFxxEdit,27,1);
  
  QLabel * inertiaFyyLabel = new QLabel( tr("I_{Fyy} (kg m^2)") );    
  QLineEdit * inertiaFyyEdit = new QLineEdit("0.28");
  benchParamLayout->addWidget(inertiaFyyLabel,28,0);
  benchParamLayout->addWidget(inertiaFyyEdit,28,1);
*/

