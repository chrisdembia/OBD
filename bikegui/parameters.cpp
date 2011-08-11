// c
#include <string>
#include <vector>

// qt
#include <QtGui>
#include <QVTKWidget.h>

// vtk, to be used possibly with plotting a 2D bike
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkContext2D.h>

// whipple
#include "whipple.h"
#include "whippleutils.h"

// gui
#include "myqwhipple.h"
#include "parameters.h"

WhippleParameter::WhippleParameter(std::vector<MyQWhipple*>* qb, QWidget
    *parent) : QWidget(parent) {

  // bring the bike pointer(s) from mainwindow over to parameters.
  qbikes = qb;
  qbikes->push_back( new MyQWhipple("bike1") );
  bidx = 0;


  // struct from whipple.h, to hold the parambox values
  gswp = new WhippleParams; // gyrostat whipple parameters
  gswptemp = new WhippleParams;
  mjwp = new MJWhippleParams; // Meijaard whipple parameters
  mjwptemp = new MJWhippleParams;

  wasGyroSelectedBefore = false; 
  wasMeijSelectedBefore = false;

  layout = new QGridLayout(this);
  setLayout(layout);


  initBikeBox();
  
  paramTypeBox = new QGroupBox( tr("Choose parameter type"), this);
  paramTypeLayout = new QVBoxLayout(paramTypeBox);
  paramTypeBox->setLayout(paramTypeLayout);
  paramComboBox = new QComboBox(this);
  paramTypeLayout->addWidget(paramComboBox);
  layout->addWidget(paramTypeBox,0,1,1,1);

  paramComboBox->addItem(tr("Gyrostat parameters"));
//  paramComboBox->addItem(tr("Franke parameters"));
  paramComboBox->addItem(tr("Meijaard parameters"));


  initParamBox();
  drawGyroParamBox();
  
  connect(paramComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(
        paramBoxSlot(int)) );
  
  initQDrawBox();
}

void WhippleParameter::initBikeBox() {
//  bikeComboBox = new QComboBox(
  bikeBox = new QGroupBox( tr("Bicycle list"), this);
  layout->addWidget(bikeBox,0,0,2,1);

  bikeLayout = new QGridLayout(bikeBox);
  bikeBox->setLayout(bikeLayout);

  bikeListView = new QListView(this);
  for (unsigned int i = 0; i < qbikes->size(); i++) {
    bikeList << qbikes->at(i)->getName().c_str();
  }
  bikeListModel = new QStringListModel(bikeList);
  bikeListView->setModel(bikeListModel);

  bikeListSelection = bikeListView->selectionModel();
  /*
  connect(bikeListSelection,SIGNAL(selectionChanged(const QItemSelection&
          selected, const QItemSelection& deselected)), this,
      SLOT(bikeListSelectionSlot(const QItemSelection& selected, const
          QItemSelection& deselected)));
          */
  connect(bikeListSelection,SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(bikeListCurrentSlot(const QModelIndex&, const QModelIndex&)));

  addBikeButton = new QToolButton(bikeBox);
  addBikeButton->setText( tr("Add bike"));
  connect(addBikeButton, SIGNAL(clicked()), this, SLOT(addBikeSlot()));
  removeBikeButton = new QToolButton(bikeBox);
  removeBikeButton->setText( tr("Remove bike"));
  connect(removeBikeButton, SIGNAL(clicked()), this, SLOT(removeBikeSlot()));

  bikeLayout->addWidget(bikeListView,0,0,1,2);
  bikeLayout->addWidget(addBikeButton,1,0);
  bikeLayout->addWidget(removeBikeButton,1,1);
}

void WhippleParameter::addBikeSlot() {
  // prevent suggesting duplicate filenames
  int bikenum = qbikes->size();
  QString bikename = QString("bike%1").arg(bikenum);
  bool isunique = false;
  while (!isunique) {
    bikenum++;
    bikename = QString("bike%1").arg(bikenum);
    isunique = true;
    for (unsigned int i = 0; i < qbikes->size(); i++) {
      if (!bikename.compare( bikeList.at(i) )) {
        isunique = false;
      }
    }
  }
  bool ok;
  QString text = QInputDialog::getText(this, tr("New bicycle"),
      tr("Enter the name for a new bicycle:"), QLineEdit::Normal,
      bikename, &ok);

  if (ok) {
    qbikes->push_back( new MyQWhipple(text.toStdString()));
    bikeList << text.toStdString().c_str();
    // i'm not happy with this solution; the listview does not update otherwise
 //   bikeListModel->insertRows(bikeListModel->rowCount(), 1);
//    bikeListModel->setData(bikeListModel->rowCount(), QVariant(text.toStdString().c_str()));
    bikeListModel->setStringList(bikeList);

  }
}

void WhippleParameter::removeBikeSlot() {

  int bikeidx = bikeListView->selectionModel()->selectedRows().at(0).row();
  std::string bikename = qbikes->at(bikeidx)->getName();
  QMessageBox::StandardButton clickedbutton = QMessageBox::question(this, tr("Removing a bicycle"), tr("Are you sure you want to delete the bicycle ") + bikename.c_str() + " ?", QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
  if (clickedbutton == QMessageBox::Ok) {
    // remove a vector from the vectors
    //delete qbikes->at(bikeidx);
    // MEMORY MANAGEMENT ISSUES. THE BIKE IS NOT PROPERLY DELETED.
    qbikes->erase(qbikes->begin()+bikeidx);
    // update the qlistmodel and the qlistview
    bikeList.removeAt(bikeidx);
    bikeListModel->setStringList(bikeList);
    //bikeListModel->removeRows(bikeidx,1);
  } 
}

void WhippleParameter::bikeListSelectionSlot(const QItemSelection& selected, const QItemSelection& deselected) {
  std::cout << "heyyo" << std::endl;
  std::cout << selected.indexes().at(0).column() << std::endl;
}

void WhippleParameter::bikeListCurrentSlot(const QModelIndex& current, const QModelIndex& previous) {
  // change the parameters and bike view
  // using old bike index

  if (qbikes->at(bidx)->getParamType() == 0) {
    qbikes->at(bidx)->gyroFileLabelText = gyroFileLabel->text();
    bidx = current.row();  
    *gswp = *(qbikes->at(bidx)->getGyroParams());
    updateGyroParamEdits();
    gyroFileLabel->setText(qbikes->at(bidx)->gyroFileLabelText);
  } else if (qbikes->at(bidx)->getParamType() == 1) {
    qbikes->at(bidx)->meijFileLabelText = meijFileLabel->text();  
    bidx = current.row();  
    *mjwp = *(qbikes->at(bidx)->getMeijParams());
    updateMeijParamEdits();
    meijFileLabel->setText(qbikes->at(bidx)->meijFileLabelText);
  } else {
    std::cerr << "error in bikeListCurrentSlot" << std::endl;
  }
  // set the parameter type correctly
  if (qbikes->at(bidx)->getParamType() != paramComboBox->currentIndex()) {
    paramComboBox->setCurrentIndex(qbikes->at(bidx)->getParamType());
  } 
  qbikes->at(bidx)->QDraw2D(drawScene);
}

/*
void WhippleParameter::initBikePerBox() {

}
*/

void WhippleParameter::initParamBox() {
  paramBox = new QGroupBox( tr("Set parameter values"), this);
  paramBox->setMinimumSize(300,600);
  layout->addWidget(paramBox,1,1,1,1);
}

void WhippleParameter::initDrawBox() {
  drawBox = new QGroupBox( tr("Drawing"), this);
  layout->addWidget(drawBox,0,2,1,1);
  QVBoxLayout* drawLayout = new QVBoxLayout(drawBox);
  drawBox->setLayout(drawLayout);

  QVTKWidget* qvtkWidget = new QVTKWidget(drawBox);
  qvtkWidget->GetInteractor()->Initialize();
  vtkSmartPointer<vtkContextView> drawVTKView = vtkSmartPointer<vtkContextView>::New();
  drawVTKView->SetInteractor(qvtkWidget->GetInteractor());
  qvtkWidget->SetRenderWindow(drawVTKView->GetRenderWindow());
  qbikes->at(bidx)->Draw2D(drawVTKView->GetContext());

  drawLayout->addWidget(qvtkWidget);

  qvtkWidget->GetRenderWindow()->Render();

  qvtkWidget->GetInteractor()->Start();
}

void WhippleParameter::initQDrawBox() {
  drawBox = new QGroupBox( tr("Drawing"), this);
  layout->addWidget(drawBox,0,2,2,1);
  drawLayout = new QVBoxLayout(drawBox);
  drawBox->setLayout(drawLayout);

  saveDrawButton = new QToolButton(drawBox);
  saveDrawButton->setText( tr("Save bicycle drawing"));
  connect(saveDrawButton,SIGNAL(clicked()), this, SLOT(saveDrawSlot()));
  drawLayout->addWidget(saveDrawButton);

  drawView = new QGraphicsView(this);
  drawView->setRenderHints(QPainter::Antialiasing);
  drawLayout->addWidget(drawView);
  drawScene = new QGraphicsScene(drawView);
  drawScene->setBackgroundBrush(Qt::white);
  drawView->setScene(drawScene);
  qbikes->at(bidx)->QDraw2D(drawScene);
  drawView->setMinimumSize(400,200);
  drawView->fitInView(drawView->sceneRect(),Qt::KeepAspectRatioByExpanding);
  //drawView->scale(100,100);


}

void WhippleParameter::saveDrawSlot() {
  QString fdirname = QFileDialog::getSaveFileName(this, tr("Save drawing"), QDir::currentPath(), tr("Postscript (*.ps)") );
  // from whippleutils.h
  if (!fdirname.isEmpty()) {
  }

}

void WhippleParameter::paramBoxSlot(int index) {
  delete paramBox;
  initParamBox();
  
  if (index == 0) {
    qbikes->at(bidx)->setParamType(index);
    drawGyroParamBox();  
  }
  else if (index == 2) { // Franke parameters
   // QGridLayout * frankeParamLayout = new QGridLayout;
  //  frankeParamLayout->addWidget(new QLabel( tr("Not available. Bug the developers!") ),0,0);
   // paramBox->setLayout(frankeParamLayout);
  }
  else if (index == 1) { // Meijaard parameters
    qbikes->at(bidx)->setParamType(index);
    drawMeijParamBox();
  }
  // IMPLEMENT EVERYTHING ELSE.
  // manage deleting objects etc.
}

void WhippleParameter::drawGyroParamBox() {
  // putting gyrostat parameter widgets into a QGridLayout
//  gyroParamLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  
  QGroupBox * gyroParamFileBox = 
    new QGroupBox( tr("File management"),paramBox );
  QScrollArea * gyroParamModScroll = new QScrollArea(paramBox);
  QGroupBox * gyroParamModBox = 
    new QGroupBox( tr("Modify parameters"), gyroParamModScroll );
  
  gyroParamFileBox->setMaximumHeight(175);
  // putting benchmark parameter widgets into a QGridLayout
  QVBoxLayout * gyroParamLayout = new QVBoxLayout(paramBox);
  QGridLayout * gyroParamFileLayout = new QGridLayout(gyroParamFileBox);
  QGridLayout * gyroParamModLayout = new QGridLayout(gyroParamModBox);

  gyroParamFileBox->setLayout(gyroParamFileLayout);
  gyroParamModBox->setLayout(gyroParamModLayout);
  gyroParamLayout->addWidget(gyroParamFileBox);
  gyroParamLayout->addWidget(gyroParamModScroll);  
  
  paramBox->setLayout(gyroParamLayout);

  defineGyroStrings();
  
  // load parameters label
  QLabel * gyroLoadLabel = new QLabel("current file:",gyroParamFileBox);

  // load parameters button
  QToolButton * gyroLoadButton = new QToolButton(gyroParamFileBox);
  gyroLoadButton->setText("Load");
  connect(gyroLoadButton,SIGNAL(clicked()),this,SLOT(gyroLoadSlot()));
  
  // save parameters button
  QToolButton * gyroSaveButton = new QToolButton(gyroParamFileBox);
  gyroSaveButton->setText("Save");
  connect(gyroSaveButton,SIGNAL(clicked()),this,SLOT(gyroSaveSlot()));

  // save as parameters button
  QToolButton * gyroSaveAsButton = new QToolButton(gyroParamFileBox);
  gyroSaveAsButton->setText("Save As");
  connect(gyroSaveAsButton,SIGNAL(clicked()),this,SLOT(gyroSaveAsSlot()));

  // line edit to show open file
  gyroFileLabel = new QLabel(gyroParamFileBox);
  gyroFileLabel->setTextFormat(Qt::RichText);
 
  // the value for the file label is set further down 
  
  // use benchmark parameters button
  QToolButton * gyroBenchParamsButton = new QToolButton(gyroParamFileBox);
  gyroBenchParamsButton->setText("Use benchmark parameters");
  connect(gyroBenchParamsButton, SIGNAL(clicked()), this,
      SLOT(setGyroBenchParametersSlot()) );
  
  // error message box
  gyroErrorText = new QTextEdit("Error messages will appear here.",gyroParamFileBox);

  gyroParamFileLayout->addWidget(gyroLoadLabel,0,0);
  gyroParamFileLayout->addWidget(gyroLoadButton,0,1);
  gyroParamFileLayout->addWidget(gyroSaveButton,0,2);
  gyroParamFileLayout->addWidget(gyroSaveAsButton,0,3);
  gyroParamFileLayout->addWidget(gyroFileLabel,1,0,1,4);
  gyroParamFileLayout->addWidget(gyroBenchParamsButton,2,0,1,4);
  gyroParamFileLayout->addWidget(gyroErrorText,4,0,3,4);  
  
  for (int i = 0; i < NgyroParams; i++) {
    gyroParamLabels[i] = new QLabel( gyroParamStrings[i], gyroParamModBox ); // dont need to save these
    gyroParamLabels[i]->setToolTip( tr(gyroParamToolTips[i].c_str() ) );

    gyroParamEdits[i] = new QLineEdit( gyroParamModBox);
    gyroParamEdits[i]->setAlignment(Qt::AlignRight);
    gyroParamEdits[i]->setValidator(new QDoubleValidator(-999.0, 999.0, 5, gyroParamEdits[i]));

    gyroParamModLayout->addWidget(gyroParamLabels[i],i,0);
    gyroParamModLayout->addWidget(gyroParamEdits[i],i,1,1,2);
  }
  
// initialize parameters to the benchmark bicycle.
  if (!wasGyroSelectedBefore) {
    setGyroBenchParametersSlot();
    wasGyroSelectedBefore = true;
  }
  else {
    QFontMetrics what(font());
    gyroFileLabel->setText(what.elidedText("<b>" + gyrofdirname + "</b>",Qt::ElideLeft,gyroFileLabel->width()));

    sendGyroParamToBike();
  }

  for (int i = 0; i < NgyroParams; i++) {
    connect(gyroParamEdits[i],
        SIGNAL(editingFinished()),this,SLOT(gyroAsteriskSlot()));
  }

  gyroParamModScroll->setWidget(gyroParamModBox);
}

void WhippleParameter::updateGyroParamEdits(void) {
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
  
  for (int k = 0; k < NgyroParams; k++) {
    gyroParamEdits[k]->setText( QString("%1").arg(gyroParamValues[k]) );
  }
  
}

void WhippleParameter::gyroAsteriskSlot() {
  // add an asterisk to the dialog box
  if ( !gyroFileLabel->text().endsWith("*</b>") ) {
    QString qstr = gyroFileLabel->text();
    gyroFileLabel->setText( qstr.insert( qstr.lastIndexOf("<"), "*") );
  }

  // revalidate and save the parameters  
  gswptemp->rr =    gyroParamEdits[0]->text().toDouble();
  gswptemp->rrt =   gyroParamEdits[1]->text().toDouble();   
  gswptemp->rf =    gyroParamEdits[2]->text().toDouble(); 
  gswptemp->rft =   gyroParamEdits[3]->text().toDouble(); 
  gswptemp->lr =    gyroParamEdits[4]->text().toDouble(); 
  gswptemp->ls =    gyroParamEdits[5]->text().toDouble(); 
  gswptemp->lf =    gyroParamEdits[6]->text().toDouble(); 
  gswptemp->mr =    gyroParamEdits[7]->text().toDouble(); 
  gswptemp->mf =    gyroParamEdits[8]->text().toDouble(); 
  gswptemp->ICyy =  gyroParamEdits[9]->text().toDouble(); 
  gswptemp->IDxx =  gyroParamEdits[10]->text().toDouble();
  gswptemp->IDyy =  gyroParamEdits[11]->text().toDouble();
  gswptemp->IDzz =  gyroParamEdits[12]->text().toDouble();
  gswptemp->IDxz =  gyroParamEdits[13]->text().toDouble();
  gswptemp->IExx =  gyroParamEdits[14]->text().toDouble();
  gswptemp->IEyy =  gyroParamEdits[15]->text().toDouble();
  gswptemp->IEzz =  gyroParamEdits[16]->text().toDouble();
  gswptemp->IExz =  gyroParamEdits[17]->text().toDouble();
  gswptemp->IFyy =  gyroParamEdits[18]->text().toDouble();
  gswptemp->lrx =   gyroParamEdits[19]->text().toDouble();
  gswptemp->lrz =   gyroParamEdits[20]->text().toDouble();
  gswptemp->lfx =   gyroParamEdits[21]->text().toDouble();
  gswptemp->lfz =   gyroParamEdits[22]->text().toDouble();
  gswptemp->g =     gyroParamEdits[23]->text().toDouble();

  sendGyroParamToBike();  
}

void WhippleParameter::sendGyroParamToBike() {

  // set the parameters to be used by the Whipple bike object.
  bool validparameters = false;
  try {  // NOTE: QT WILL NOT CATCH EXCEPTIONS FOR US, SO ALL EXCEPTION-THROWING CODE MUST BE IN MY OWN TRY..CATCH
    validparameters = qbikes->at(bidx)->getBike()->setParameters( gswptemp, true);
  }
  catch (const char * errmsg) {
    gyroErrorText->setTextColor( Qt::red );
    gyroErrorText->setText( tr(errmsg) );
  }
  catch (...) {
    std::cerr << "Caught an unknown exception in WhippleParameter::sendGyroParamToBike(). Tried to run bike->setParameters( WhippleParams*, bool ), which should only throw const char *'s." << std::endl;
  }
  
  if (validparameters) {
    *gswp = *gswptemp;
    qbikes->at(bidx)->setGyroParams(gswp);
    gyroErrorText->setTextColor( Qt::darkGreen );
    gyroErrorText->setText( tr("Parameters set.") );
  }
  updateGyroParamEdits();
  if (wasGyroSelectedBefore) {
    drawScene->clear();
    qbikes->at(bidx)->QDraw2D(drawScene);
  }
}

void WhippleParameter::gyroLoadSlot() {
  QString fdirname = QFileDialog::getOpenFileName(this, tr("Load file"), QDir::currentPath() );
  if (!fdirname.isEmpty()) {
    gyrofdirname = fdirname;
    // from whippleutils.h
    readWhippleParams(gswptemp, gyrofdirname.toStdString().c_str() );
    sendGyroParamToBike(); 
    QFontMetrics qfm1(font());
    gyroFileLabel->setText("<b>" + qfm1.elidedText(gyrofdirname,Qt::ElideLeft,int(.9*gyroFileLabel->width())) + "</b>");
    
  }
}

void WhippleParameter::gyroSaveSlot() {
  if (gyrofdirname.isEmpty()) {
    gyroSaveAsSlot();
  }
  else {
  qbikes->at(bidx)->getBike()->writeParameters( gyrofdirname.toStdString().c_str()
      );
  }
}

void WhippleParameter::gyroSaveAsSlot() { QString fdirname = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("Text file (*, *.txt, *.dat,...);;Any file (*)") );
  // from whippleutils.h
  if (!fdirname.isEmpty()) {
    gyrofdirname = fdirname;
    qbikes->at(bidx)->getBike()->writeParameters(
        gyrofdirname.toStdString().c_str() );
    QFontMetrics qfm1(font());
    gyroFileLabel->setText("<b>" + qfm1.elidedText(gyrofdirname,Qt::ElideLeft,int(.9*gyroFileLabel->width())) + "</b>");
  }
}

void WhippleParameter::setGyroBenchParametersSlot() {
  // in whippleutils.h
  MJWhippleParams *mjwptemptemp = new MJWhippleParams;
  setBenchmarkParameters( mjwptemptemp);
  convertParameters( gswptemp, mjwptemptemp);
  delete mjwptemptemp;

  sendGyroParamToBike();

  gyrofdirname.clear();
  gyroFileLabel->setText("<b>benchmark</b>");
  
} 

void WhippleParameter::drawMeijParamBox() {
  QGroupBox * meijParamFileBox =
    new QGroupBox( tr("File management"),paramBox );
  QScrollArea * meijParamModScroll = new QScrollArea(paramBox);
  QGroupBox * meijParamModBox =
    new QGroupBox( tr("Modify parameters"), meijParamModScroll);
  meijParamFileBox->setMaximumHeight(175);
  // putting benchmark parameter widgets into a QGridLayout
  QVBoxLayout * meijParamLayout = new QVBoxLayout(paramBox);
  QGridLayout * meijParamFileLayout = new QGridLayout(meijParamFileBox);
  QGridLayout * meijParamModLayout = new QGridLayout(meijParamModBox);

  meijParamFileBox->setLayout(meijParamFileLayout);
  meijParamModBox->setLayout(meijParamModLayout);
  meijParamLayout->addWidget(meijParamFileBox);
  
  meijParamLayout->addWidget(meijParamModScroll);  
  paramBox->setLayout(meijParamLayout);

//  meijParamLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  // DO I NEED THE ABOVE?
 
  defineMeijStrings();
  
  // load parameters label
  QLabel * meijLoadLabel = new QLabel("current file:",meijParamFileBox);

  // load parameters button
  QToolButton * meijLoadButton = new QToolButton(meijParamFileBox);
  meijLoadButton->setText("Load");
  connect(meijLoadButton,SIGNAL(clicked()),this,SLOT(meijLoadSlot()));
  
  // save parameters button
  QToolButton * meijSaveButton = new QToolButton(meijParamFileBox);
  meijSaveButton->setText("Save");
  connect(meijSaveButton,SIGNAL(clicked()),this,SLOT(meijSaveSlot()));

  // save as parameters button
  QToolButton * meijSaveAsButton = new QToolButton(meijParamFileBox);
  meijSaveAsButton->setText("Save As");
  connect(meijSaveAsButton,SIGNAL(clicked()),this,SLOT(meijSaveAsSlot()));

  // line edit to show open file
  meijFileLabel = new QLabel(meijParamModBox);
  meijFileLabel->setTextFormat(Qt::RichText);
  // the value for the file label is set further down 
  
  // use benchmark parameters button
  QToolButton * meijBenchParamsButton = new QToolButton(meijParamFileBox);
  meijBenchParamsButton->setText("Use benchmark parameters");
  connect(meijBenchParamsButton, SIGNAL(clicked()), this, SLOT(setMeijBenchParametersSlot()) );
 
  // convert Meijaard parameters to Gyrostat parameters
  QToolButton * meijToGyroButton = new QToolButton(meijParamFileBox);
  meijToGyroButton->setText("Convert Meij. to Gyro params");

  // error message box
  meijErrorText = new QTextEdit("Parameters are okay.",meijParamFileBox);

 
  meijParamFileLayout->addWidget(meijLoadLabel,0,0);
  meijParamFileLayout->addWidget(meijLoadButton,0,1);
  meijParamFileLayout->addWidget(meijSaveButton,0,2);
  meijParamFileLayout->addWidget(meijSaveAsButton,0,3);
  meijParamFileLayout->addWidget(meijFileLabel,1,0,1,4);
  meijParamFileLayout->addWidget(meijBenchParamsButton,2,0,1,4);
  meijParamFileLayout->addWidget(meijToGyroButton,3,0,1,4);
  meijParamFileLayout->addWidget(meijErrorText,4,0,3,4);  
  for (int i = 0; i < NmeijParams; i++) {
    meijParamLabels[i] =
      new QLabel( meijParamStrings[i], meijParamModBox); // dont need to save these
    meijParamLabels[i]->setToolTip( tr(meijParamToolTips[i].c_str() ) );
    
    meijParamEdits[i] = new QLineEdit(meijParamModBox);
    meijParamEdits[i]->setAlignment(Qt::AlignRight);
    meijParamEdits[i]->setValidator(new QDoubleValidator(-999.0, 999.0, 5, meijParamEdits[i]));

    meijParamModLayout->addWidget(meijParamLabels[i],i,0);
    meijParamModLayout->addWidget(meijParamEdits[i],i,1,1,2);
  }
  meijParamEdits[4]->setReadOnly(true); // forward velocity is not a real parameter
  // initialize parameters to the benchmark bicycle.
  if (!wasMeijSelectedBefore) {
    setMeijBenchParametersSlot();
    wasMeijSelectedBefore = true;
  }
  else {
    updateMeijParamEdits();
    QFontMetrics qfm1(font());
    meijFileLabel->setText("<b>" + qfm1.elidedText(meijfdirname,Qt::ElideLeft,int(.9*meijFileLabel->width())) + "</b>");
  }

  for (int i = 0; i < NmeijParams; i++) {
    connect(meijParamEdits[i], SIGNAL(editingFinished()),this,SLOT(meijAsteriskSlot()));
  }

  
  meijParamModScroll->setWidget(meijParamModBox);
}

void WhippleParameter::updateMeijParamEdits(void) {
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
  for (int k = 0; k < NmeijParams; k++) {
    meijParamEdits[k]->setText( QString("%1").arg(meijParamValues[k]) );
  }

}

void WhippleParameter::meijAsteriskSlot() {
  // add an asterisk to the dialog box
  if ( !meijFileLabel->text().endsWith("*</b>") ) {
    QString qstr = meijFileLabel->text();
    meijFileLabel->setText( qstr.insert( qstr.lastIndexOf("<"), "*") );
  }

  // revalidate and save the parameters  
  mjwptemp->w = meijParamEdits[0]->text().toDouble();
  mjwptemp->c = meijParamEdits[1]->text().toDouble();
  mjwptemp->lambda = meijParamEdits[2]->text().toDouble();
  mjwptemp->g = meijParamEdits[3]->text().toDouble();
  mjwptemp->rrt = meijParamEdits[5]->text().toDouble();
  mjwptemp->rft = meijParamEdits[6]->text().toDouble();
  mjwptemp->rr = meijParamEdits[7]->text().toDouble();
  mjwptemp->mr = meijParamEdits[8]->text().toDouble();
  mjwptemp->IRxx = meijParamEdits[9]->text().toDouble();
  mjwptemp->IRyy = meijParamEdits[10]->text().toDouble();
  mjwptemp->xb = meijParamEdits[11]->text().toDouble();
  mjwptemp->zb = meijParamEdits[12]->text().toDouble();
  mjwptemp->mb = meijParamEdits[13]->text().toDouble();
  mjwptemp->IBxx = meijParamEdits[14]->text().toDouble();
  mjwptemp->IByy = meijParamEdits[15]->text().toDouble();
  mjwptemp->IBzz = meijParamEdits[16]->text().toDouble();
  mjwptemp->IBxz = meijParamEdits[17]->text().toDouble();
  mjwptemp->xh = meijParamEdits[18]->text().toDouble();
  mjwptemp->zh = meijParamEdits[19]->text().toDouble();
  mjwptemp->mh = meijParamEdits[20]->text().toDouble();
  mjwptemp->IHxx = meijParamEdits[21]->text().toDouble();
  mjwptemp->IHyy = meijParamEdits[22]->text().toDouble();
  mjwptemp->IHzz = meijParamEdits[23]->text().toDouble();
  mjwptemp->IHxz = meijParamEdits[24]->text().toDouble();
  mjwptemp->rf = meijParamEdits[25]->text().toDouble();
  mjwptemp->mf = meijParamEdits[26]->text().toDouble();
  mjwptemp->IFxx = meijParamEdits[27]->text().toDouble();
  mjwptemp->IFyy = meijParamEdits[28]->text().toDouble();
  
  sendMeijParamToBike();
}

void WhippleParameter::sendMeijParamToBike() {

  bool validparameters = false;
  meijErrorText->setTextColor( Qt::black );
  meijErrorText->setText( tr("Error messages appear here.") );
  // set the parameters to be used by the Whipple bike object.
  try {  // NOTE: QT WILL NOT CATCH EXCEPTIONS FOR US, SO ALL EXCEPTION-THROWING CODE MUST BE IN MY OWN TRY..CATCH

   // MAY TRY TO IMPLEMENT TRYCATCH INTO CONVERTPARAMETERS INSTEAD. NOTE THAT THERE ARE ERRORS BEING CAUGHT BY GYROSTAT SET THAT ARE NOT BEING CAUGHT BY WHIPPLE SET
    validparameters = validateMJParameters( mjwptemp, true);
  }
  catch (const char* errmsg) {
    meijErrorText->setTextColor( Qt::red );
    meijErrorText->setText( tr(errmsg) );
  }
  catch (...) {
    std::cerr << "Caught an unknown exception in WhippleParameter::sendMeijParamToBike(). Tried to run bike->validateMJParameters( MJWhippleParams*, bool ), which should only throw const char *'s." << std::endl;
  }

  if (validparameters) { // from whippleutils.h
    WhippleParams *gswptemptemp = new WhippleParams;
    convertParameters( gswptemptemp, mjwptemp);
    try {
      validparameters = qbikes->at(bidx)->getBike()->setParameters(
          gswptemptemp, true);
    }
    catch (...) {
      meijErrorText->setTextColor( Qt::red );
      meijErrorText->setText( tr("Caught an error after internally converting Meijaard parameters to Gyrostat parameters.") );
    }
    
    if (validparameters) {
      *mjwp = *mjwptemp;
      qbikes->at(bidx)->setMeijParams(mjwp);
      meijErrorText->setTextColor( Qt::darkGreen );
      meijErrorText->setText( tr("Parameters set.") );
    }
    delete gswptemptemp;
  }
  updateMeijParamEdits();
  if (wasMeijSelectedBefore) {
    drawScene->clear();
    qbikes->at(bidx)->QDraw2D(drawScene);
  }
}

void WhippleParameter::meijLoadSlot() {
  QString fdirname = QFileDialog::getOpenFileName(this, tr("Load file"), QDir::currentPath() );
  if (!fdirname.isEmpty()) {
    meijfdirname = fdirname;
    readMJWhippleParams(mjwptemp, meijfdirname.toStdString().c_str() );
    updateMeijParamEdits(); 
    QFontMetrics qfm1(font());
    meijFileLabel->setText("<b>" + qfm1.elidedText(meijfdirname,Qt::ElideLeft,int(.9*meijFileLabel->width())) + "</b>");
  }
}

void WhippleParameter::meijSaveSlot() {
  if (meijfdirname.isEmpty()) {
    meijSaveAsSlot();
  } else {
    writeMJWhippleParams(mjwp, meijfdirname.toStdString().c_str() );
  }
}

void WhippleParameter::meijSaveAsSlot() {
  QString fdirname = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), tr("Text file (*, *.txt, *.dat,...);;Any file (*)") );
  // from whippleutils.h
  if (!fdirname.isEmpty()) {
    meijfdirname = fdirname;
    writeMJWhippleParams(mjwp, meijfdirname.toStdString().c_str() );
    QFontMetrics qfm1(font());
    meijFileLabel->setText("<b>" + qfm1.elidedText(meijfdirname,Qt::ElideLeft,int(.9*meijFileLabel->width())) + "</b>");
  }
}

void WhippleParameter::setMeijBenchParametersSlot() {
  // in whippleutils.h
  setBenchmarkParameters(mjwp);

  // update line edits
  updateMeijParamEdits();
  
  // set the displayed file name appropriately (the data is built in)
  meijfdirname.clear();
  meijFileLabel->setText("<b>benchmark</b>");
}

void WhippleParameter::defineMeijStrings() {
  // MEIJAARD PARAMETERS
  meijParamStrings[0] = "w (m)";
  meijParamStrings[1] = "c (m)";
  meijParamStrings[2] = QString(0x03BB) + " (rad)";
  meijParamStrings[3] = "g (m/s<sup>2</sup>)";
  meijParamStrings[4] = "v (m/s)";
  meijParamStrings[5] = "r<sub>Rt</sub> (m)";
  meijParamStrings[6] = "r<sub>Ft</sub> (m)";
  meijParamStrings[7] = "r<sub>R</sub> (m)";
  meijParamStrings[8] = "m<sub>R</sub> (kg)";
  meijParamStrings[9] = "I<sub>Rxx</sub> (kg m<sup>2</sup>)";
  meijParamStrings[10] = "I<sub>Ryy</sub> (kg m<sup>2</sup>)";
  meijParamStrings[11] = "x<sub>B</sub> (m)";
  meijParamStrings[12] = "z<sub>B</sub> (m)";
  meijParamStrings[13] = "m<sub>B</sub> (kg)";
  meijParamStrings[14] = "I<sub>Bxx</sub> (kg m<sup>2</sup>)";
  meijParamStrings[15] = "I<sub>Byy</sub> (kg m<sup>2</sup>)";
  meijParamStrings[16] = "I<sub>Bzz</sub> (kg m<sup>2</sup>)";
  meijParamStrings[17] = "I<sub>Bxz</sub> (kg m<sup>2</sup>)";
  meijParamStrings[18] = "x<sub>H</sub> (m)";
  meijParamStrings[19] = "z<sub>H</sub> (m)";
  meijParamStrings[20] = "m<sub>H</sub> (m)";
  meijParamStrings[21] = "I<sub>Hxx</sub> (kg m<sup>2</sup>)";
  meijParamStrings[22] = "I<sub>Hyy</sub> (kg m<sup>2</sup>)";
  meijParamStrings[23] = "I<sub>Hzz</sub> (kg m<sup>2</sup>)";
  meijParamStrings[24] = "I<sub>Hxz</sub> (kg m<sup>2</sup>)";
  meijParamStrings[25] = "r<sub>F</sub> (m)";
  meijParamStrings[26] = "m<sub>F</sub> (kg)";
  meijParamStrings[27] = "I<sub>Fxx</sub> (kg m<sup>2</sup>)";
  meijParamStrings[28] = "I<sub>Fyy</sub> (kg m<sup>2</sup>)";

  meijParamToolTips[0] = "Wheel base: distance between contact patches of the front and rear wheels.";
  for (int i = 1; i < NmeijParams; i++ ) {
  meijParamToolTips[i] = "EDIT";
  }
} 

void WhippleParameter::defineGyroStrings() {
  // GYROSTAT PARAMETERS
  gyroParamStrings[0] = "r<sub>R</sub> (m)";
  gyroParamStrings[1] = "r<sub>Rt</sub> (m)";
  gyroParamStrings[2] = "r<sub>F</sub> (m)";
  gyroParamStrings[3] = "r<sub>Ft</sub> (m)";
  gyroParamStrings[4] = "l<sub>R</sub> (m)";
  gyroParamStrings[5] = "l<sub>S</sub> (m)";
  gyroParamStrings[6] = "l<sub>F</sub> (m)";
  gyroParamStrings[7] = "m<sub>R</sub> (kg)";
  gyroParamStrings[8] = "m<sub>F</sub> (kg)";
  gyroParamStrings[9] = "I<sub>Cyy</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[10] = "I<sub>Dxx</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[11] = "I<sub>Dyy</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[12] = "I<sub>Dzz</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[13] = "I<sub>Dxz</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[14] = "I<sub>Exx</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[15] = "I<sub>Eyy</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[16] = "I<sub>Ezz</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[17] = "I<sub>Exz</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[18] = "I<sub>Fyy</sub> (kg-m<sup>2</sup>)";
  gyroParamStrings[19] = "l<sub>Rx</sub> (m)";
  gyroParamStrings[20] = "l<sub>Rz</sub> (m)";
  gyroParamStrings[21] = "l<sub>Fx</sub> (m)";
  gyroParamStrings[22] = "l<sub>Fz</sub> (m)";
  gyroParamStrings[23] = "g (m/s<sup>2</sup>)";

  gyroParamToolTips[0] = "rear wheel radius";
  for (int i = 1; i < NgyroParams; i++) {
    gyroParamToolTips[i] = "EDIT";
  }
} 
