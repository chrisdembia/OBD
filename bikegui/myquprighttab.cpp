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
  saveEigDataButton->setToolTip( tr("Will create a space-delimited text file for each bicycle in the eigenvalue plot, with the name of the bicycles appended to the filename you provide.") );
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
  int NuprightBikes = 0;
  for (unsigned int i = 0; i < qbikes->size(); i++) {
    if (qbikes->at(i)->getDoUpright()) {
      NuprightBikes++;
    } // if
  } // for i

  // Add multiple line plots, setting the colors etc
  int NeigPerBike = 4;
  int red;
  int green;
  int blue;
  //eigPlotVTKChart->ClearPlots();
  unsigned int Nplots = eigPlotVTKChart->GetNumberOfPlots();
  for (unsigned int i = 0; i < Nplots; i++) {
    eigPlotVTKChart->RemovePlot(i);
  } // for i
  eigPlotQVTKW->GetRenderWindow()->Render();
  vtkSmartPointer<vtkPlot> lineplot;
  int idx = 0;
  for (unsigned int i = 0; i < qbikes->size(); i++) {
    if (qbikes->at(i)->getDoUpright()) {
      qbikes->at(i)->calcUpright(upOpts);
      red = 200*(idx/NuprightBikes - 1) + 50;
      green = 200*idx/NuprightBikes + 50;
      blue = 200*idx/NuprightBikes + 50;
      for (int j = 0; j < NeigPerBike; j++) { // j < NeigPerBike
        lineplot = eigPlotVTKChart->AddPlot(vtkChart::POINTS); // ::LINES
        lineplot->SetInput(qbikes->at(i)->GetUprightTable(), 0, j+1);
        lineplot->SetColor(red, green, blue);
        //lineplot->SetWidth(.5);
      }
      idx++;
    } // if
  } // for i

  eigPlotQVTKW->GetRenderWindow()->Render();

  // SHOULD MAKE DESTRUCTORS

} // updateEigPlotSlot()

void MyQUprightTab::saveEigPlotSlot(void) {
  QString dir = QFileDialog::getSaveFileName(this, tr("Save plot"),
      QDir::currentPath());
  w2i->SetInput(eigPlotQVTKW->GetRenderWindow());
  writerPS->SetFileName(dir.toStdString().c_str());
  writerPS->Write();

} // saveEigPlotSlot()

void MyQUprightTab::saveEigDataSlot(void) {
  QString dir = QFileDialog::getSaveFileName(this, tr("Save data"),
      QDir::currentPath(), tr("Text file (*)"));
  for (unsigned int i = 0; i < qbikes->size(); i++) {
    if (qbikes->at(i)->getDoUpright()) {
      qbikes->at(i)->printUprightData(dir);
    } // if
  } // for i
} // saveEigDataSlot()

void MyQUprightTab::calcIntersections(void) {
  // for each qbikeobject
  // store intersections IN the qbike
  // bifurcations
  // zero passings
  // draw in the corresponding lines
  // Jason emailed Chris some useful files for doing this.
}

/*
def sort_modes(evals, evecs):
    '''Sort eigenvalues and eigenvectors into weave, capsize, caster modes.

    Parameters
    ----------
    evals : ndarray, shape (n, 4)
        eigenvalues
    evecs : ndarray, shape (n, 4, 4)
        eigenvectors

    Returns
    -------
    weave['evals'] : ndarray, shape (n, 2)
        The eigen value pair associated with the weave mode.
    weave['evecs'] : ndarray, shape (n, 4, 2)
        The associated eigenvectors of the weave mode.
    capsize['evals'] : ndarray, shape (n,)
        The real eigenvalue associated with the capsize mode.
    capsize['evecs'] : ndarray, shape(n, 4, 1)
        The associated eigenvectors of the capsize mode.
    caster['evals'] : ndarray, shape (n,)
        The real eigenvalue associated with the caster mode.
    caster['evecs'] : ndarray, shape(n, 4, 1)
        The associated eigenvectors of the caster mode.

    This only works on the standard bicycle eigenvalues, not necessarily on any
    general eigenvalues for the bike model (e.g. there isn't always a distinct weave,
    capsize and caster). Some type of check unsing the derivative of the curves
    could make it more robust.

    '''
    evalsorg = np.zeros_like(evals)
    evecsorg = np.zeros_like(evecs)
    # set the first row to be the same
    evalsorg[0] = evals[0]
    evecsorg[0] = evecs[0]
    # for each speed
    for i, speed in enumerate(evals):
        if i == evals.shape[0] - 1:
            break
        # for each current eigenvalue
        used = []
        for j, e in enumerate(speed):
            try:
                x, y = np.real(evalsorg[i, j].nominal_value), np.imag(evalsorg[i, j].nominal_value)
            except:
                x, y = np.real(evalsorg[i, j]), np.imag(evalsorg[i, j])
            # for each eigenvalue at the next speed
            dist = np.zeros(4)
            for k, eignext in enumerate(evals[i + 1]):
                try:
                    xn, yn = np.real(eignext.nominal_value), np.imag(eignext.nominal_value)
                except:
                    xn, yn = np.real(eignext), np.imag(eignext)
                # distance between points in the real/imag plane
                dist[k] = np.abs(((xn - x)**2 + (yn - y)**2)**0.5)
            if np.argmin(dist) in used:
                # set the already used indice higher
                dist[np.argmin(dist)] = np.max(dist) + 1.
            else:
                pass
            evalsorg[i + 1, j] = evals[i + 1, np.argmin(dist)]
            evecsorg[i + 1, :, j] = evecs[i + 1, :, np.argmin(dist)]
            # keep track of the indices we've used
            used.append(np.argmin(dist))
    weave = {'evals' : evalsorg[:, 2:], 'evecs' : evecsorg[:, :, 2:]}
    capsize = {'evals' : evalsorg[:, 1], 'evecs' : evecsorg[:, :, 1]}
    caster = {'evals' : evalsorg[:, 0], 'evecs' : evecsorg[:, :, 0]}
    return weave, capsize, caster
    */
