#ifndef PARAMETERS_H
#define PARAMETERS_H

#define NMEIJPARAMS 29
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
   * Called when a user clicks the button "Use benchmark parameters".
   * */
  void setBenchmarkParametersSlot(void);

  /**
   * Chris should really rename this method, assigns updated values from mjwp to the Whipple bike.
   * */
  void setMeijaardParams(Whipple * bike);

  /**
   * Update the display to the gui with updated Meijaard parameter values.
   * */
  void updateMeijaardParamEdits(void);

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

  // Whipple bicycle stuffs
  
  /**
   * Pointer to a whipple bike created in the MainWindow class. We have a pointer here so that we can modify the parameters that the bicycle(s) use(s).
   * */
  Whipple * bike;
  /**
   * Structure, declared in Whipple.h, to hold the default (Gyrostat) parameters for Whipple objects.
   * */
  WhippleParams * gswp;

  // Meijaard parameter stuffs
  /**
   * Structure, declared in Whipple.h, to hold the input parameters if Meijaard parmeters are selected from the QtComboBox.
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
   * Holds the values, ripped from struct MJWhippleParams mjwp, for use with the GUI input of Meijaard parameters.
   * */
  double meijParamValues[NMEIJPARAMS];
};
#endif
