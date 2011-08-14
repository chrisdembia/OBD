#ifndef PARAMETERS_H
#define PARAMETERS_H

// c
#include <vector>

// qt
#include <QtGui>

// OBD
#include "whipple.h"

// gui
class MyQWhipple;

// define
#define NMEIJPARAMS 29
#define NGYROPARAMS 24

class WhippleParameter : public QWidget {
  Q_OBJECT

public:
  /**
   * Constructor for the GUI's left panel for parameter inputs.
   * */
  WhippleParameter(std::vector<MyQWhipple*>* qb, QWidget *parent = 0);

private slots:
  void initCheckBox();
  void uprightCheckSlot(int);
  void simCheckSlot(int);

  void addBikeSlot();

  void removeBikeSlot();

  void bikeListSelectionSlot(const QItemSelection& selected, const QItemSelection& deselected);

  void bikeListCurrentSlot(const QModelIndex& current, const QModelIndex& previous);

  void saveDrawSlot();

  /**
   *
   * */
  void paramBoxSlot(int index);

  /**
   *
   * */
  void gyroAsteriskSlot();

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
  void meijAsteriskSlot();

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
  QCheckBox *uprightCheck;
  QCheckBox *simCheck;
  QGroupBox *checkBox;
  QVBoxLayout *checkLayout;

  std::vector<MyQWhipple*>* qbikes;
  unsigned int bidx;
  /**
   * Qt Vertical layout holding the QtComboBox and parameter QtGroupBox
   * */
  QGridLayout *layout;

  QGroupBox *bikeBox;
  QGridLayout *bikeLayout;
  QListView *bikeListView;
  QStringList bikeList;
  QStringListModel *bikeListModel;
  QItemSelectionModel *bikeListSelection;

  QToolButton *addBikeButton;
  QToolButton *removeBikeButton;

  QGroupBox *drawBox;
  QVBoxLayout *drawLayout;
  QGraphicsView *drawView;
  QGraphicsScene *drawScene;
  QToolButton *saveDrawButton;


  /**
   * QtComboBox to allow a user to select which types of parameters to use.
   * */
  QComboBox *paramComboBox;

  /*
   *
   * */
  void initBikeBox(void);

  void initDrawBox(void);

  void initQDrawBox(void);


  /**
   * QtGroupBox where a user can load, save, or modify parameters.
   * */
  QGroupBox *paramBox;

  QVBoxLayout *paramTypeLayout;

  QGroupBox *paramTypeBox;

  /**
   *
   * */
  void initParamBox();

  // Whipple bicycle stuffs

  /**
   * Pointer to a whipple bike created in the MainWindow class. We have a pointer here so that we can modify the parameters that the bicycle(s) use(s).
   * */


  // Gyrostat parameter stuffs

  /**
   *
   * */
  bool wasGyroSelectedBefore;

  /**
   * Structure, declared in Whipple.h, to hold the default (Gyrostat) parameters for Whipple objects.
   * */
  WhippleParams *gswp;
  WhippleParams *gswptemp;

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
  QLabel *gyroFileLabel;
  
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
  QLabel *gyroParamLabels[NGYROPARAMS];

  /**
   *
   * */
  QLineEdit *gyroParamEdits[NGYROPARAMS];

  /**
   *
   * */
  QString gyroParamStrings[NGYROPARAMS];

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
  QLabel *meijFileLabel;

  /**
   *
   * */
  QString meijfdirname;

  /**
   * Structure, declared in Whipple.h, to hold the input parameters if Meijaard parmeters are selected from the QtComboBox.
zo
   * */
  MJWhippleParams *mjwp;
  MJWhippleParams *mjwptemp;

  /**
   * The number of Meijaard parameters.

   * */
  const static int NmeijParams = NMEIJPARAMS; // static?
  
  /**
   * Holds the QLabel objects used for the GUI input of Meijaard parameters.
   * */
  QLabel *meijParamLabels[NMEIJPARAMS];

  /**
   * Holds the QLineEdit objects used for the GUI input of Meijaard parameters.
   * */
  QLineEdit *meijParamEdits[NMEIJPARAMS];
  
  /**
   * Holds the string names for use with QLabel objects for the GUI input of Meijaard parameters.
   * */
  QString meijParamStrings[NMEIJPARAMS];

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
  QTextEdit *meijErrorText;
};
#endif
