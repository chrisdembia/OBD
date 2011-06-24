#ifndef PARAMETERS_H
#define PARAMETERS_H

#define NBENCHPARAMS 29
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
  WhippleParameter(Whipple * bike, QWidget *parent = 0);

private slots:
  /**
   * Called when a user clicks the button "Use (default) benchmark parameters".
   * */
  void setBenchmarkParametersSlot(void);

  /**
   * Chris should really rename this method, assigns updated values from mjwp to the Whipple bike.
   * */
  void setBenchParams(Whipple * bike);

  /**
   * Update the display to the gui with updated MJ/benchmark parameter values.
   * */
  void updateBenchParams(void);

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

  // MJ/benchmark parameter stuffs
  /**
   * Structure, declared in Whipple.h, to hold the input parameters if MJ/Benchmark parmeters are selected from the QtComboBox.
   * */
  MJWhippleParams * mjwp;
  
  /**
   * The number of MJ/benchmark parameters.
   * */
  const static int NbenchParams = NBENCHPARAMS; // static?
  
  /**
   * Holds the QLabel objects used for the GUI input of MJ/benchmark parameters.
   * */
  QLabel * benchParamLabels[NBENCHPARAMS];

  /**
   * Holds the QLineEdit objects used for the GUI input of MJ/benchmark parameters.
   * */
  QLineEdit * benchParamEdits[NBENCHPARAMS];
  
  /**
   * Holds the string names for use with QLabel objects for the GUI input of MJ/benchmark parameters.
   * */
  std::string benchParamStrings[NBENCHPARAMS];

  /**
   * Holds the values, ripped from struct MJWhippleParams mjwp, for use with the GUI input of MJ/benchmark parameters.
   * */
  double benchParamValues[NBENCHPARAMS];
};
#endif
