/* whippleeig.cpp
 *
 * Copyright (C) 2010-2011 Dale Lukas Peterson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <getopt.h>
#include "gslVecUtils.h"
#include "whipple.h"
#include "whippleutils.h"
#include "OBDConfig.h"

// Structor to store the options
typedef struct {
  char outfolder[512];
  int N;
  double vi, vf;
} evalOptions;

// Forward declaration
void processOptions(int argc, char ** argv, evalOptions * opt , Whipple * bike);
void writeEvalRecord_dt(const char * filename);

int main(int argc, char ** argv)
{
  Whipple * bb = new Whipple();
  std::string filename;

  evalOptions * opt = new evalOptions;
  // default parameters
  opt->outfolder[0] = '\0';
  opt->N = 201; opt->vi = 0.0; opt->vf = 10.0;

  // Process command line options
  processOptions(argc, argv, opt, bb);

  // Write parameters
  filename = opt->outfolder; filename += "eigenvalue_parameters.txt";
  bb->writeParameters(filename.c_str());
  // Write data record file.
  filename = opt->outfolder; filename += "eval_record.py";
  writeEvalRecord_dt(filename.c_str());
  // Open data file
  filename = opt->outfolder; filename += "eigenvalues.dat";
  std::ofstream OutputFile(filename.c_str(), std::ios::binary);

  // Vector to store range of speeds to calculate eigenvalues
  gsl_vector * speed = linspaceN(opt->vi, opt->vf, opt->N);

  bb->u1 = 0.0;
  bb->u3 = 0.0;
  bb->evalConstants();

  for (int i = 0; i < opt->N; ++i) {
    bb->u5 = -gsl_vector_get(speed, i)/(bb->rf+bb->rft);
    bb->calcEvals();
    OutputFile.write((char *) gsl_vector_ptr(speed, i), sizeof(double));
    OutputFile.write((char *) bb->fourValues, 4*sizeof(double));
  } // for i
  std::cout << "Eigenvalue data written to " << opt->outfolder << '\n';

  // Close files and free memory
  OutputFile.close();
  delete bb;
  delete opt;
  gsl_vector_free(speed);
  return 0;
} // main

void processOptions(int argc, char ** argv, evalOptions * opt, Whipple * bike)
{
  int c, option_index;
  while (1) {
    static struct option long_options[] = {
      {"help",          no_argument,       0, 'h'},
      {"bmparams",      required_argument, 0, 'm'},
      {"params",        required_argument, 0, 'p'},
      {"pitch_ig",      required_argument, 0, 'g'},
      {"output",        required_argument, 0, 'o'},
      {"points",        required_argument, 0, 'n'},
      {"vi",            required_argument, 0, 'i'},
      {"vf",            required_argument, 0, 'f'},
      {0, 0, 0, 0}};

    c = getopt_long(argc, argv, "hm:p:g:o:n:i:f:", long_options, &option_index);

    if (c == -1) //Detect the end of the options.
      break;

    if (c == 'h') {
      std::cout <<
argv[0] << " Version " << OBD_VERSION_MAJOR << "." << OBD_VERSION_MINOR <<
" commit " << OBD_VERSION_COMMIT << "\n"
"usage: " << argv[0] << " [OPTION]\n\n"
"Mandatory arguments to long options are mandatory for short options too.\n\n"
"  -h, --help                         display this help and exit.\n"
"  -m, --Meijaard-parameters=pfile    Meijaard bike parameters\n"
"  -p, --parameters=pfile             native bike parameters\n"
"  -g, --pitch_ig=FP_NUMBER           initial guess for the pitch root finder\n"
"  -n, --points=201                   # of points to compute e-vals at\n"
"  -i, --vi=0.0                       initial speed\n"
"  -f, --vf=10.0                      final speed\n"
"  -o, --output=outputfile            write eigenvalues to outputfile\n\n";
      exit(0);
    } else if (c == 'm') {
      MJWhippleParams * mjbike = new MJWhippleParams;
      WhippleParams * b = new WhippleParams;
      readMJWhippleParams(mjbike, optarg);
      convertParameters(b, mjbike);
      bike->setParameters(b);
      bike->evalConstants();
      bike->eoms();
      bike->computeOutputs();
      delete mjbike;
      delete b;
    } else if (c == 'p') {
      WhippleParams * b = new WhippleParams;
      readWhippleParams(b, optarg);
      bike->setParameters(b);
      bike->evalConstants();
      bike->eoms();
      bike->computeOutputs();
      delete b;
    } else if (c == 'g') {
      bike->q2 = atof(optarg);
      bike->calcPitch();
      bike->eoms();
      bike->computeOutputs();
    } else if (c == 'n')
      opt->N = atoi(optarg);
    else if (c == 'i')
      opt->vi = atof(optarg);
    else if (c == 'f')
      opt->vf = atof(optarg);
    else if (c == 'o')
      strcpy(opt->outfolder, optarg);
    else {
      std::cout << "Invalid option." << '\n';
      abort();
    }
  } // while()
} // processOptions()

void writeEvalRecord_dt(const char * filename)
{
  std::ofstream fp(filename, std::ios::out);
  if (fp.is_open()) {
    fp << "import numpy as np\n";
    fp << "eval_dt = np.dtype([('v', np.float64),\n"
          "                    ('lambda1', np.float64),\n"
          "                    ('lambda2', np.float64),\n"
          "                    ('lambda3', np.float64),\n"
          "                    ('lambda4', np.float64)])\n";
    fp.close();
  } else {
    std::cerr << "Unable to open " << filename << "for writing.\n";
    std::cerr << "Aborting.\n";
    exit(0);
  }
} // writeEvalRecord_dt()
