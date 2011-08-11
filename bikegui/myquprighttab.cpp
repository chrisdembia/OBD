// c
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>
#include <QVTKWidget.h>

// vtk
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkChartXY.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
// vtk saving plot
#include <vtkWindowToImageFilter.h>
#include <vtkPostScriptWriter.h>
#include <vtkPNGWriter.h>

// OBD
#include "whipple.h"
#include "whippleutils.h"
#include "OBDConfig.h"
#include "gslVecUtils.h"

// gui
#include "myquprighttab.h"
#include "myqwhipple.h"

MyQUprightTab::MyQUprightTab(std::vector<MyQWhipple*>* qb, QWidget *parent) :
  QWidget(parent) {

  qbikes = qb;
  // upOpts is a structure for run parameters for upright stability eigenvalue
  // plots. initialize parameters to default values.
  upOpts.N = 201;
  upOpts.pitchguess = 0.0;
  upOpts.vi = 0.0;
  upOpts.vf = 10.0;  
 
  // set up gui
  uprightSetLayout = new QGridLayout;
  uprightSetBox = new QGroupBox( tr("Settings") );
  uprightSetBox->setLayout(uprightSetLayout);
  uprightSetBox->setMaximumWidth(200);
  uprightTopLayout = new QHBoxLayout;
  uprightTopLayout->addWidget(uprightSetBox);
  setLayout(uprightTopLayout);
  //uprightSetLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  uprightSetLayout->setVerticalSpacing(0);  
  // update eig button
  updateEigButton = new QToolButton(this);
  updateEigButton->setText( tr("Update eigenvalue plot") );
  connect(updateEigButton, SIGNAL(clicked()), this, SLOT(updateEigPlotSlot()) );
  uprightSetLayout->addWidget(updateEigButton, 0, 0, 1, 2);
  
  // Save buttons
  saveEigPlotButton = new QToolButton(this);
  saveEigPlotButton->setText( tr("Save plot") );
  connect(saveEigPlotButton, SIGNAL(clicked()), this, SLOT(saveEigPlotSlot()));
  uprightSetLayout->addWidget(saveEigPlotButton, 1, 0); //, Qt::AlignLeft);

  saveEigDataButton = new QToolButton(this);
  saveEigDataButton->setText( tr("Save data") );
  connect(saveEigDataButton, SIGNAL(clicked()), this, SLOT(saveEigDataSlot()));
  uprightSetLayout->addWidget(saveEigDataButton, 1, 1); //, Qt::AlignRight);

  // INCLUDE SOME INPUT VALIDATION HERE
  // Pitch guess
  uprightSetLayout->addWidget(new QLabel( tr("pitch guess (rad)")), 2, 0);
  pitchGuessEdit = new QLineEdit;
  pitchGuessEdit->setToolTip( tr("blank for default") );
  pitchGuessEdit->setText( QString("%1").arg(upOpts.pitchguess) );
  pitchGuessEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(pitchGuessEdit, 2, 1);
  
  // Number of evaluation points
  uprightSetLayout->addWidget(new QLabel( tr("number of speeds") ), 3, 0);
  nEvalPointsEdit = new QLineEdit;
  nEvalPointsEdit->setToolTip( tr("The number of points, between the first and last speeds, at which the eigenvalues will be calculated and plotted."));
  nEvalPointsEdit->setText( QString("%1").arg(upOpts.N) );
  nEvalPointsEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(nEvalPointsEdit, 3, 1);
 
  
  // Initial speed
  uprightSetLayout->addWidget(new QLabel( tr("first speed (m/s)") ), 4, 0);
  firstSpeedEdit = new QLineEdit;
  firstSpeedEdit->setText( QString("%1").arg(upOpts.vi) );
  firstSpeedEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(firstSpeedEdit, 4, 1);
  
  // Final speed
  uprightSetLayout->addWidget(new QLabel( tr("last speed (m/s)") ), 5, 0);
  lastSpeedEdit = new QLineEdit;
  lastSpeedEdit->setText( QString("%1").arg(upOpts.vf) );
  lastSpeedEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(lastSpeedEdit, 5, 1);

  // QVTK set up and initialization
  eigPlotQVTKW = new QVTKWidget(this);
  eigPlotQVTKW->GetRenderWindow()->Render();
  eigPlotQVTKW->setMinimumSize(500, 200);
  //  chartView->GetWidget()->setMinimumSize(800, 800);
  //        uprightTopLayout->addWidget(chartView->GetWidget());
  //QGroupBox *eigVTKBox = new QGroupBox( tr("heyyo") );
  //eigVTKBox->setLayout(eigPlotLayout);
  uprightTopLayout->addWidget(eigPlotQVTKW);  

  // vtk setup
  // vtk
  eigPlotVTKView = vtkSmartPointer<vtkContextView>::New();
  eigPlotVTKView->SetInteractor(eigPlotQVTKW->GetInteractor());
  eigPlotQVTKW->SetRenderWindow(eigPlotVTKView->GetRenderWindow());
  eigPlotVTKChart = vtkSmartPointer<vtkChartXY>::New();
  eigPlotVTKView->GetScene()->AddItem(eigPlotVTKChart);
  // the line smoothing will only happen in a/certain conditions are met
  eigPlotQVTKW->GetRenderWindow()->LineSmoothingOn();
  eigPlotQVTKW->GetInteractor()->Initialize();
  eigPlotQVTKW->GetInteractor()->Start();

  writerPS = vtkSmartPointer<vtkPostScriptWriter>::New();
  writerPNG = vtkSmartPointer<vtkPNGWriter>::New();
  w2i = vtkSmartPointer<vtkWindowToImageFilter>::New();
  w2i->SetInput(eigPlotQVTKW->GetRenderWindow());
  writerPS->SetInput(w2i->GetOutput());
  writerPNG->SetInput(w2i->GetOutput());
} // MyQUprightTab()

/*MyQUprightTab::~MyQUprightTab() {
  eigPlotVTKView->Delete();
  eigPlotVTKChart->Delete();
  writerPS->Delete();
  writerPNG->Delete();
  w2i->Delete();
}*/

void MyQUprightTab::updateEigPlotSlot(void) {

  // update the parameters
  upOpts.N = nEvalPointsEdit->text().toInt();
  upOpts.pitchguess = pitchGuessEdit->text().toDouble();
  upOpts.vi = firstSpeedEdit->text().toDouble();
  upOpts.vf = lastSpeedEdit->text().toDouble();
  
  // call the qbikes
  std::vector< vtkSmartPointer<vtkTable> > tables;
  tables.resize(qbikes->size());
  for (unsigned int i = 0; i < qbikes->size(); i++) {
    qbikes->at(i)->calcUpright(upOpts);
    tables[i] = qbikes->at(i)->GetUprightTable();
  } // for i
  
  // Add multiple line plots, setting the colors etc
  int NeigPerBike = 4;
  //eigPlotVTKChart->ClearPlots();
  for (unsigned int i = 0; i < qbikes->size()*NeigPerBike; i++) {
    eigPlotVTKChart->RemovePlot(i);
  }
  eigPlotQVTKW->GetRenderWindow()->Render();
  vtkSmartPointer<vtkPlot> lineplot;
  for (unsigned int i = 0; i < qbikes->size(); i++) {
    for (int j = 0; j < NeigPerBike; j++) { // j < NeigPerBike
      lineplot = eigPlotVTKChart->AddPlot(vtkChart::POINTS);
      lineplot->SetInput(tables[i], 0, j+1);
      lineplot->SetColor(0, 255, 0, 255);
      lineplot->SetWidth(2.0);
    }
  }

  eigPlotQVTKW->GetRenderWindow()->Render();

  // SHOULD MAKE DESTRUCTORS
  /*for (unsigned int i = 0; i < qbikes->size(); i++) {
    tables[i]->Delete();
  }*/ // this was giving a segfault

} // updateEigPlotSlot()

void MyQUprightTab::saveEigPlotSlot(void) {
  QString dir = QFileDialog::getSaveFileName(this, tr("Save plot"),
      QDir::currentPath());
//  eigPlotQVTKW->GetRenderWindow()->Render();
  w2i->SetInput(eigPlotQVTKW->GetRenderWindow());
  writerPS->SetFileName(dir.toStdString().c_str());
  writerPS->Write();

} // saveEigPlotSlot()

void MyQUprightTab::saveEigDataSlot(void) {
/*
  filename = upOpts.outfolder; filename += "eigenvalue_parameters.txt";
  qbikes->at(0)->getBike()->writeParameters(filename.c_str());
  // Write data record file. the function is orphaned from whipple.h currently
  // allows the evaluation of data by python
  // Open data file
  filename = upOpts.outfolder; filename += "eigenvalues.dat";
  std::ofstream OutputFile( filename.c_str() ); // std::ios::binary);
  

  for (int i = 0; i < upOpts.N; ++i) {
    qbikes->at(0)->getBike()->u5 = -gsl_vector_get(speed,
        i)/(qbikes->at(0)->getBike()->rf+qbikes->at(0)->getBike()->rft);
    OutputFile << *gsl_vector_ptr(speed, i);
    for (int j = 0; j < 4; ++j) {
      OutputFile << " " << qbikes->at(0)->getBike()->fourValues[j];
    }
    OutputFile << std::endl;
  } // for i

  // Close files and free memory
  OutputFile.close();
*/
} // saveEigDataSlot()

void MyQUprightTab::calcIntersections(void) {
  // for each qbikeobject
  // store intersections IN the qbike
  // bifurcations
  // zero passings
  // draw in the corresponding lines
  // Jason emailed Chris some useful files for doing this.
}
