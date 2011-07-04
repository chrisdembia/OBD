#ifndef PARAMETERS_H
#define PARAMETERS_H

#define NMEIJPARAMS 29
#define NGYROPARAMS 24
#include <QtGui>
#include "whipple.h"

class QVBoxLayout;
class QComboBox;
class QGroupBox;

class WhippleParameter : public QWidget
{
  Q_OBJECT

public:
  /**
   * Constructor for the GUI's left panel for parameter inputs.
   * */
  WhippleParameter(Whipple * b, QWidget *parent = 0);

private slots:
  /**
   *
   * */
  void paramBoxSlot(int index);

  /**
   *
   * */
  void gyroAsteriskSlot(const QString&);

  /**
   *
   * */
  void gyroLoadSlot();

  /**
   *
   * */
  void gyroSaveSlot();

  /**
   *
   * */
  void gyroSaveAsSlot();  

  /**
   *
   * */
  void setGyroBenchParametersSlot();
  
  /**
   *
   * */
  void meijAsteriskSlot(const QString&);

  /**
   *
   * */
  void meijLoadSlot();

  /**
   *
   * */
  void meijSaveSlot();

  /**
   *
   * */
  void meijSaveAsSlot();  

  /**
   * Called when a user clicks the button "Use benchmark parameters".
   * */
  void setMeijBenchParametersSlot(void);

private:
  /**
   * Qt Vertical layout holding the QtComboBox and parameter QtGroupBox
   * */
  QVBoxLayout *layout;
  
  /**
   * QtComboBox to allow a user to select which types of parameters to use.
   * */
  QComboBox *comboBox;
  
  /**
   * QtGroupBox where a user can load, save, or modify parameters.
   * */
  QGroupBox *paramBox;

  /**
   *
   * */
  void initParamBox();
  
  // Whipple bicycle stuffs
  
  /**
   * Pointer to a whipple bike created in the MainWindow class. We have a pointer here so that we can modify the parameters that the bicycle(s) use(s).
   * */
  Whipple * bike;
  

  // Gyrostat parameter stuffs

  /**
   *
   * */
  bool wasGyroSelectedBefore;

  /**
   * Structure, declared in Whipple.h, to hold the default (Gyrostat) parameters for Whipple objects.
   * */
  WhippleParams * gswp;
  
  /**
   *
   * */
  void drawGyroParamBox();

  /**
   *
   * */
  void defineGyroStrings();

  /**
   * Update the display to the gui with updated Gyrostat parameter values.
   * */
  void updateGyroParamEdits(void);

  /**
   *
   * */
  void sendGyroParamToBike();

  /**
   *
   * */
  QLabel * gyroFileLabel;
  
  /**
   *
   * */
  QString gyrofdirname;

  /**
   *
   * */
  const static int NgyroParams = NGYROPARAMS;

  /**
   *
   * */
  QLabel * gyroParamLabels[NGYROPARAMS];

  /**
   *
   * */
  QLineEdit * gyroParamEdits[NGYROPARAMS];

  /**
   *
   * */
  std::string gyroParamStrings[NGYROPARAMS];

  /**
   *
   * */
  std::string gyroParamToolTips[NGYROPARAMS];

  /**
   *
   * */
  double gyroParamValues[NGYROPARAMS];

  /**
   *
   * */
  QTextEdit *gyroErrorText;
  // Meijaard parameter stuffs
  /**
   * This is the first time that the user has chosen Meijaard parameters from the dropdown.
   * */
  int wasMeijSelectedBefore;

/**
   *
   * */
  void drawMeijParamBox();

  /**
   *
   * */
  void defineMeijStrings();  
  
  /**
   * Update the display to the gui with updated Meijaard parameter values.
   * */
  void updateMeijParamEdits(void);

  /**
   *
   * */
  void sendMeijParamToBike();

  /**
   * 
   * */
  QLabel * meijFileLabel;

  /**
   *
   * */
  QString meijfdirname;

  /**
   * Structure, declared in Whipple.h, to hold the input parameters if Meijaard parmeters are selected from the QtComboBox.
zo
   * */
  MJWhippleParams * mjwp;

  /**
   * The number of Meijaard parameters.

   * */
  const static int NmeijParams = NMEIJPARAMS; // static?
  
  /**
   * Holds the QLabel objects used for the GUI input of Meijaard parameters.
   * */
  QLabel * meijParamLabels[NMEIJPARAMS];

  /**
   * Holds the QLineEdit objects used for the GUI input of Meijaard parameters.
   * */
  QLineEdit * meijParamEdits[NMEIJPARAMS];
  
  /**
   * Holds the string names for use with QLabel objects for the GUI input of Meijaard parameters.
   * */
  std::string meijParamStrings[NMEIJPARAMS];

  /**
   *
   * */
  std::string meijParamToolTips[NMEIJPARAMS];

  /**
   * Holds the values, ripped from struct MJWhippleParams mjwp, for use with the GUI input of Meijaard parameters.
   * */
  double meijParamValues[NMEIJPARAMS];

  /**
   *
   * */
  QTextEdit * meijErrorText;
};
#endif
