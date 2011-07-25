
#include <cstdlib>
#include <string>
#include <vector>

#include <QtGui>
// graphics

#include <QVTKWidget.h>
// plotting
#include "vtkQtLineChartView.h"
#include "vtkQtChartRepresentation.h"
#include "vtkQtTableView.h"
#include "vtkDataObjectToTable.h"
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>

#include <getopt.h>
#include "gslVecUtils.h"
#include "whipple.h"
#include "whippleutils.h"
#include "OBDConfig.h"

#include "parameters.h"
#include "mainwindow.h"
#include "myqsimtab.h"

void MainWindow::initUprightStabilityTab(void)
{

  // upOpts is a structure for run parameters for upright stability eigenvalue plots
  upOpts.outfolder[0] = '\0';
  upOpts.N = 201;
  upOpts.pitchguess = 0.0;
  upOpts.vi = 0.0;
  upOpts.vf = 10.0;  
  
  QGridLayout *uprightSetLayout = new QGridLayout;
//  uprightSetLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  uprightSetLayout->setVerticalSpacing(0);  
  QToolButton * updateEigButton = new QToolButton(uprightStabilityTab);
  updateEigButton->setText( tr("Update eigenvalue plot") );
  connect(updateEigButton, SIGNAL(clicked()), this, SLOT(updateEigPlotSlot()) );
  uprightSetLayout->addWidget(updateEigButton,0,0);
  
  // manage options
  
  uprightSetLayout->addWidget(new QLabel( tr("Save eigenvalue data"),0,0 ) );
  
  // Save button
  QToolButton * saveEigButton = new QToolButton(uprightStabilityTab);
  saveEigButton->setText( tr("Save Plot") );
  connect(saveEigButton, SIGNAL(clicked()), this, SLOT(saveEigSlot() ) );
  uprightSetLayout->addWidget(saveEigButton,0,1,Qt::AlignRight);
  
  // Edit filename for saving plot
  saveEigFilenameEdit = new QLineEdit( tr( upOpts.outfolder.c_str() ) );
  saveEigFilenameEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(saveEigFilenameEdit,1,0,1,2);
  
  // Number of evaluation points
  uprightSetLayout->addWidget(new QLabel( tr("number of evaluation points") ),3,0);
  nEvalPointsEdit = new QLineEdit;
  nEvalPointsEdit->setText( QString("%1").arg(upOpts.N) );
  nEvalPointsEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(nEvalPointsEdit,3,1);
  
  // Pitch guess
  uprightSetLayout->addWidget(new QLabel( tr("pitch guess (rad) (blank for default)") ),2,0);
  pitchGuessEdit = new QLineEdit;
  pitchGuessEdit->setText( QString("%1").arg(upOpts.pitchguess) );
  pitchGuessEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(pitchGuessEdit,2,1);
  
  // Initial speed
  uprightSetLayout->addWidget(new QLabel( tr("initial speed v_i (m/s)") ), 4,0);
  initSpeedEdit = new QLineEdit;
  initSpeedEdit->setText( QString("%1").arg(upOpts.vi) );
  initSpeedEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(initSpeedEdit,4,1);
  
  // Final speed
  uprightSetLayout->addWidget(new QLabel( tr("final speed v_f (m/s)") ), 5,0);
  finalSpeedEdit = new QLineEdit;
  finalSpeedEdit->setText( QString("%1").arg(upOpts.vf) );
  finalSpeedEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(finalSpeedEdit,5,1);

QGroupBox *uprightSetBox = new QGroupBox( tr("Settings") );

uprightSetBox->setLayout(uprightSetLayout);
uprightSetBox->setMaximumWidth(200);
 //QScrollArea *eigScroll = new QScrollArea;
 //eigScroll->setBackgroundRole(QPalette::Dark);
  
QHBoxLayout *uprightTopLayout = new QHBoxLayout;
    uprightTopLayout->addWidget(uprightSetBox);
  // QVTK set up and initialization
  eigPlotQVTKW = new QVTKWidget(uprightStabilityTab);
//  chartView->GetWidget()->setMinimumSize(800,800);
//        uprightTopLayout->addWidget(chartView->GetWidget());
//QGroupBox *eigVTKBox = new QGroupBox( tr("heyyo") );
//eigVTKBox->setLayout(eigPlotLayout);
    uprightTopLayout->addWidget(eigPlotQVTKW);  
  uprightStabilityTab->setLayout(uprightTopLayout);

eigPlotQVTKW->setMinimumSize(500,200);
//    return 0; for errors?
}

void MainWindow::saveEigSlot(void)
{
  /*QFileDialog *saveEigDirDial = new QFileDialog(uprightStabilityTab);
  saveEigDirDial->setFileMode(QFileDialog::Directory);
  saveEigDirDial->setViewMode(QFileDialog::List);
  */
  QString dir = QFileDialog::getExistingDirectory(this,tr("Choose Directory"),QDir::currentPath(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

}

void MainWindow::updateEigPlotSlot(void)
{
  // THIS STUFF BELOW IS MOSTLY COPIED
  // Set up my 2D world...
  VTK_CREATE(vtkContextView, eigPlotVTKView); // This contains a chart object
  eigPlotVTKView->SetInteractor(eigPlotQVTKW->GetInteractor());
  eigPlotQVTKW->SetRenderWindow(eigPlotVTKView->GetRenderWindow());
  
  // Create a table with some points in it...
  VTK_CREATE(vtkTable, eigPlotVTKTable);
  VTK_CREATE(vtkFloatArray, arrX);
  arrX->SetName("forward velocity (m/s)");
  eigPlotVTKTable->AddColumn(arrX);
  
//  int Nbikes = 1;
  int NeigPerBike = 4;
  std::vector<std::string> bikenames(paramWidget->getNbikes()*NeigPerBike);
  bikenames[0] = "eig1";
  bikenames[1] = "eig2";
  bikenames[2] = "eig3";
  bikenames[3] = "eig4";

  std::vector< vtkSmartPointer<vtkFloatArray> > arrY(paramWidget->getNbikes()*NeigPerBike); 
  int idx;
  for (int i = 0; i < paramWidget->getNbikes(); i++)
  {
    for (int j = 0; j < 4; j++)
    {
      idx = j + i*paramWidget->getNbikes();
      arrY[idx] = vtkSmartPointer<vtkFloatArray>::New();
      arrY[idx]->SetName(bikenames[j].c_str());
      eigPlotVTKTable->AddColumn(arrY[idx]);
    }
  }   

// WHIPPLE WHIPPLE WHIPPLE WHIPPLE 
  std::string filename;
  upOpts.outfolder = saveEigFilenameEdit->text().toStdString();
  upOpts.N = nEvalPointsEdit->text().toInt();
  upOpts.pitchguess = pitchGuessEdit->text().toDouble();
  upOpts.vi = initSpeedEdit->text().toDouble();
  upOpts.vf = finalSpeedEdit->text().toDouble();  

// TIME TO GRAB PARAMETERS FROM PARAMETER WIDGET!! must validate them, yo.

  bike->evalConstants();
  bike->eoms();
  bike->computeOutputs();

  // Write parameters
  
  filename = upOpts.outfolder; filename += "eigenvalue_parameters.txt";
  bike->writeParameters(filename.c_str());
  // Write data record file. the function is orphaned from whipple.h currently
  // allows the evaluation of data by python
//  filename = upOpts.outfolder; filename += "eval_record.py";
//  writeEvalRecord_dt(filename.c_str());
  // Open data file
  filename = upOpts.outfolder; filename += "eigenvalues.dat";
  std::ofstream OutputFile( filename.c_str() ); // std::ios::binary);
  
  // Vector to store range of speeds to calculate eigenvalues
  gsl_vector * speed = linspaceN( upOpts.vi, upOpts.vf, upOpts.N);

  bike->u1 = 0.0;
  bike->u3 = 0.0;
  bike->evalConstants();

  eigPlotVTKTable->SetNumberOfRows(upOpts.N);
  for (int i = 0; i < upOpts.N; ++i) {
    bike->u5 = -gsl_vector_get(speed, i)/(bike->rf+bike->rft);
    bike->calcEvals();
    OutputFile << *gsl_vector_ptr(speed,i);
    eigPlotVTKTable->SetValue(i, 0, *gsl_vector_ptr(speed,i) );
    for (int j = 0; j < 4; ++j)
    {
      OutputFile << " " << bike->fourValues[j];
      eigPlotVTKTable->SetValue(i, j+1, bike->fourValues[j]);
    }
    OutputFile << std::endl;
//    OutputFile.write((char *) gsl_vector_ptr(speed, i), sizeof(double));
//    OutputFile.write((char *) bike->fourValues, 4*sizeof(double));  
  } // for i




  // QVTK QVTK QVTK QVTK 
  eigPlotVTKTable->Update();
  
  // Add multiple line plots, setting the colors etc
  vtkSmartPointer<vtkChartXY> eigPlotVTKChart = vtkSmartPointer<vtkChartXY>::New();
  eigPlotVTKView->GetScene()->AddItem(eigPlotVTKChart);
  vtkPlot *eigPlotVTKLine;

  for (int i = 0; i < paramWidget->getNbikes(); i++)
  {
    for (int j = 0; j < 4; j++)
    {
      idx = j + i*paramWidget->getNbikes();
      eigPlotVTKLine = eigPlotVTKChart->AddPlot(vtkChart::LINE);
      eigPlotVTKLine->SetInput(eigPlotVTKTable, 0, j+1);
      eigPlotVTKLine->SetColor(0, 255, 0, 255);
      eigPlotVTKLine->SetWidth(2.0);
    }
  }





  // WHIPPLE WHIPPLE WHIPPLE
  std::cout << "Eigenvalue data written to ";
  if ( upOpts.outfolder.empty() )
  { 
  	std::cout << "current directory." << std::endl;
  }
  else
  {
  	std::cout << upOpts.outfolder << std::endl;
  }

  // Close files and free memory
  OutputFile.close();
  gsl_vector_free(speed);

  // SHOULD MAKE DESTRUCTORS

}
