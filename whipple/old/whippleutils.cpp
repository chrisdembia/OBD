#include "whippleutils.h"

// Converts Meijaard's parameter set to my parameter set
void convertParameters(WhippleParams * bout, const MJWhippleParams * bin)
{
  // wheel dimensions are the same in both models
  bout->rr = bin->rr;
  bout->rrt = bin->rrt;
  bout->rf = bin->rf;
  bout->rft = bin->rft;

  // converting w, c, lambda to lr, ls, lf
  bout->lr = (bin->c + bin->w)*cos(bin->lambda) + (-bin->rr - bin->rrt)*sin(bin->lambda);
  bout->ls = bin->w*sin(bin->lambda) + (bin->rr + bin->rrt - bin->rf - bin->rft)*cos(bin->lambda);
  bout->lf = (bin->rf + bin->rft)*sin(bin->lambda) - bin->c*cos(bin->lambda);

  // converting mass parameters
  bout->mc = bin->mr;
  bout->md = bin->mb;
  bout->me = bin->mh;
  bout->mf = bin->mf;

  bout->ICxx = bin->IRxx;
  bout->ICyy = bin->IRyy;

  bout->IDxx = bin->IBxx*pow(cos(bin->lambda), 2)+bin->IBzz*pow(sin(bin->lambda), 2)-2*bin->IBxz*sin(bin->lambda)*cos(bin->lambda);
  bout->IDyy = bin->IByy;
  bout->IDzz = bin->IBxx*pow(sin(bin->lambda), 2) +bin->IBzz*pow(cos(bin->lambda), 2)+2*bin->IBxz*sin(bin->lambda)*cos(bin->lambda);
  bout->IDxz = bin->IBxz+sin(bin->lambda)*(bin->IBxx*cos(bin->lambda)-2*bin->IBxz*sin(bin->lambda)-bin->IBzz*cos(bin->lambda));

  bout->IExx = bin->IHxx*pow(cos(bin->lambda), 2)+bin->IHzz*pow(sin(bin->lambda), 2) - 2*bin->IHxz*sin(bin->lambda)*cos(bin->lambda);
  bout->IEyy = bin->IHyy;
  bout->IEzz = bin->IHxx*pow(sin(bin->lambda), 2)+bin->IHzz*pow(cos(bin->lambda), 2)+2*bin->IHxz*sin(bin->lambda)*cos(bin->lambda);
  bout->IExz = bin->IHxz+sin(bin->lambda)*(bin->IHxx*cos(bin->lambda)-2*bin->IHxz*sin(bin->lambda)-bin->IHzz*cos(bin->lambda));

  bout->IFxx = bin->IFxx;
  bout->IFyy = bin->IFyy;

  bout->lrx = cos(bin->lambda)*bin->xb - sin(bin->lambda)*(bin->zb + bin->rr + bin->rrt);
  bout->lrz = sin(bin->lambda)*bin->xb + cos(bin->lambda)*(bin->zb + bin->rr + bin->rrt);

  bout->lfx = cos(bin->lambda)*(-bin->w + bin->xh) - sin(bin->lambda)*(bin->zh + bin->rf + bin->rft);
  bout->lfz = sin(bin->lambda)*(-bin->w + bin->xh) + cos(bin->lambda)*(bin->zh + bin->rf + bin->rft);

  //bout->lambda = bin->lambda;
  bout->g = bin->g;
} // convertParameters()

void setBenchmarkParameters(MJWhippleParams * bike)
{
  bike->w = 1.02;
  bike->c = 0.08;
  bike->lambda = M_PI/10.0;
  bike->g = 9.81;
  bike->rrt =  0.0;
  bike->rft =  0.0;
  bike->rr = 0.3;
  bike->mr = 2.0;
  bike->IRxx = 0.0603;
  bike->IRyy = 0.12;
  bike->xb = 0.3;
  bike->zb = -0.9;
  bike->mb = 85.0;
  bike->IBxx = 9.2;
  bike->IByy = 11.0;
  bike->IBzz = 2.8;
  bike->IBxz = 2.4;
  bike->xh = 0.9;
  bike->zh = -0.7;
  bike->mh = 4.0;
  bike->IHxx = 0.05892;
  bike->IHyy = 0.06;
  bike->IHzz = 0.00708;
  bike->IHxz = -0.00756;
  bike->rf = 0.35;
  bike->mf = 3.0;
  bike->IFxx = 0.1405;
  bike->IFyy = 0.28;
} // setBenchmarkParameters()

void readMJWhippleParams(MJWhippleParams *mjbike, const char *filename)
{
  ifstream fp(filename);
  char line[256];
  char *param, *valc;
  double val;

  if (fp.is_open()) {
    for (fp.getline(line, 255); fp.good(); fp.getline(line, 255)) {
      if (line[0] == '%' || line[0] == '#' || line[0] == '\n')
        continue;
    
      // Get the parameter name
      param = strtok(line, "=");
      if (param == NULL) {
        cerr <<
"File read error parsing parameter name." << endl <<
"Ensure file is formatted with lines like: " << endl << 
"'rr=0.30', exluding the single quotes, and no spaces anywhere." << endl <<
"Exiting." << endl;
        exit(0);
      } // if
      // Get the value of the parameter
      valc = strtok(NULL, "\n");
      if (valc == NULL) {
        cerr <<
"File read error parsing parameter value." << endl <<
"Ensure file is formatted with lines like: " << endl << 
"'rr=0.30', exluding the single quotes, and no spaces anywhere." << endl <<
"Exiting." << endl;
        exit(0);
      } // if
      val = atof(valc);  // Convert string to double

      if (strcmp(param, "rr") == 0)
        mjbike->rr = val;
      else if (strcmp(param, "rrt") == 0)
        mjbike->rrt = val;
      else if (strcmp(param, "rf") == 0)
        mjbike->rf = val;
      else if (strcmp(param, "rft") == 0)
        mjbike->rft = val;
      else if (strcmp(param, "w") == 0)
        mjbike->w = val;
      else if (strcmp(param, "c") == 0)
        mjbike->c = val;
      else if (strcmp(param, "lambda") == 0)
        mjbike->lambda = val;
      else if (strcmp(param, "mr") == 0)
        mjbike->mr = val;
      else if (strcmp(param, "mb") == 0)
        mjbike->mb = val;
      else if (strcmp(param, "mh") == 0)
        mjbike->mh = val;
      else if (strcmp(param, "mf") == 0)
        mjbike->mf = val;
      else if (strcmp(param, "IRxx") == 0)
        mjbike->IRxx = val;
      else if (strcmp(param, "IRyy") == 0)
        mjbike->IRyy = val;
      else if (strcmp(param, "IBxx") == 0)
        mjbike->IBxx = val;
      else if (strcmp(param, "IByy") == 0)
        mjbike->IByy = val;
      else if (strcmp(param, "IBzz") == 0)
        mjbike->IBzz = val;
      else if (strcmp(param, "IBxz") == 0)
        mjbike->IBxz = val;
      else if (strcmp(param, "IHxx") == 0)
        mjbike->IHxx = val;
      else if (strcmp(param, "IHyy") == 0)
        mjbike->IHyy = val;
      else if (strcmp(param, "IHzz") == 0)
        mjbike->IHzz = val;
      else if (strcmp(param, "IHxz") == 0)
        mjbike->IHxz = val;
      else if (strcmp(param, "IFxx") == 0)
        mjbike->IFxx = val;
      else if (strcmp(param, "IFyy") == 0)
        mjbike->IFyy = val;
      else if (strcmp(param, "xb") == 0)
        mjbike->xb = val;
      else if (strcmp(param, "zb") == 0)
        mjbike->zb = val;
      else if (strcmp(param, "xh") == 0)
        mjbike->xh = val;
      else if (strcmp(param, "zh") == 0)
        mjbike->zh = val;
      else if (strcmp(param, "g") == 0)
        mjbike->g = val;
    } // for
  } else {
    cerr << "Unable to open " << filename << ". Exiting." << endl;
    exit(0);
  } // if
  fp.close();
} // readMJWhippleParams()

void readWhippleParams(WhippleParams * bike, const char *filename)
{
  ifstream fp(filename);
  char line[256];
  char *param, *valc;
  double val;

  if (fp.is_open()) {
    for (fp.getline(line, 255); fp.good(); fp.getline(line, 255)) {
      if (line[0] == '%' || line[0] == '#' || line[0] == '\n')
        continue;
    
      // Get the parameter name
      param = strtok(line, "=");
      if (param == NULL) {
        cerr <<
"File read error parsing parameter name." << endl <<
"Ensure file is formatted with lines like: " << endl << 
"'rr=0.30', exluding the single quotes, and no spaces anywhere." << endl <<
"Exiting." << endl;
        exit(0);
      } // if
      // Get the value of the parameter
      valc = strtok(NULL, "\n");
      if (valc == NULL) {
        cerr <<
"File read error parsing parameter value." << endl <<
"Ensure file is formatted with lines like: " << endl << 
"'rr=0.30', exluding the single quotes, and no spaces anywhere." << endl <<
"Exiting." << endl;
        exit(0);
      } // if
      val = atof(valc);  // Convert string to double

    if (strcmp(param, "rr") == 0)
      bike->rr = val;
    else if (strcmp(param, "rrt") == 0)
      bike->rrt = val;
    else if (strcmp(param, "rf") == 0)
      bike->rf = val;
    else if (strcmp(param, "rft") == 0)
      bike->rft = val;
    else if (strcmp(param, "lr") == 0)
      bike->lr = val;
    else if (strcmp(param, "ls") == 0)
      bike->ls = val;
    else if (strcmp(param, "lf") == 0)
      bike->lf = val;
    else if (strcmp(param, "mc") == 0)
      bike->mc = val;
    else if (strcmp(param, "md") == 0)
      bike->md = val;
    else if (strcmp(param, "me") == 0)
      bike->me = val;
    else if (strcmp(param, "mf") == 0)
      bike->mf = val;
    else if (strcmp(param, "ICxx") == 0)
      bike->ICxx = val;
    else if (strcmp(param, "ICyy") == 0)
      bike->ICyy = val;
    else if (strcmp(param, "IDxx") == 0)
      bike->IDxx = val;
    else if (strcmp(param, "IDyy") == 0)
      bike->IDyy = val;
    else if (strcmp(param, "IDzz") == 0)
      bike->IDzz = val;
    else if (strcmp(param, "IDxz") == 0)
      bike->IDxz = val;
    else if (strcmp(param, "IExx") == 0)
      bike->IExx = val;
    else if (strcmp(param, "IEyy") == 0)
      bike->IEyy = val;
    else if (strcmp(param, "IEzz") == 0)
      bike->IEzz = val;
    else if (strcmp(param, "IExz") == 0)
      bike->IExz = val;
    else if (strcmp(param, "IFxx") == 0)
      bike->IFxx = val;
    else if (strcmp(param, "IFyy") == 0)
      bike->IFyy = val;
    else if (strcmp(param, "lrx") == 0)
      bike->lrx = val;
    else if (strcmp(param, "lrz") == 0)
      bike->lrz = val;
    else if (strcmp(param, "lfx") == 0)
      bike->lfx = val;
    else if (strcmp(param, "lfz") == 0)
      bike->lfz = val;
    else if (strcmp(param, "g") == 0)
      bike->g = val;
    } // for
  } else {
    cerr << "Unable to open " << filename << ". Exiting." << endl;
    exit(0);
  } // if
  fp.close();
} // readWhippleParams()

void readState(double * state, const char *filename)
{
  ifstream fp(filename);
  char line[256];
  char *param, *valc;
  double val;

  if (fp.is_open()) {
    for (fp.getline(line, 255); fp.good(); fp.getline(line, 255)) {
      if (line[0] == '%' || line[0] == '#' || line[0] == '\n')
        continue;
    
      // Get the parameter name
      param = strtok(line, "=");
      if (param == NULL) {
        cerr <<
"File read error parsing parameter name." << endl <<
"Ensure file is formatted with lines like: " << endl << 
"'rr=0.30', exluding the single quotes, and no spaces anywhere." << endl <<
"Exiting." << endl;
        exit(0);
      } // if
      // Get the value of the parameter
      valc = strtok(NULL, "\n");
      if (valc == NULL) {
        cerr <<
"File read error parsing parameter value." << endl <<
"Ensure file is formatted with lines like: " << endl << 
"'rr=0.30', exluding the single quotes, and no spaces anywhere." << endl <<
"Exiting." << endl;
        exit(0);
      } // if
      val = atof(valc);  // Convert string to double

      if (strcmp(param, "yaw") == 0)
        state[0] = val;
      else if (strcmp(param, "lean") == 0)
        state[1] = val;
      else if (strcmp(param, "steer") == 0)
        state[2] = val;
      else if (strcmp(param, "rw") == 0)
        state[3] = val;
      else if (strcmp(param, "fw") == 0)
        state[4] = val;
      else if (strcmp(param, "x") == 0)
        state[5] = val;
      else if (strcmp(param, "y") == 0)
        state[6] = val;
      else if (strcmp(param, "leanrate") == 0)
        state[7] = val;
      else if (strcmp(param, "steerrate") == 0)
        state[8] = val;
      else if (strcmp(param, "fwrate") == 0)
        state[9] = val;
    } // for 
  } // if

  fp.close();
} // readState

/*
void readIntegrationParams(BikeParams *bike, const char *filename)
{
  FILE *fp = fopen(filename, "r");
  char line[255], *status, *param, *valc;

  if (fp == NULL) {
    printf("Unable to open file %s. Exiting.\n", filename);
    exit(0);
  }

  for (status = fgets(line, 255, fp);
       status != 0;
       status = fgets(line, 255, fp)) {

    // Valid comment lines start with '%', '#', or are empty
    if (line[0] == '%' || line[0] == '#' || line[0] == '\n')
      continue;

    param = strtok(line, "=");
    if (param == NULL) {
      printf("File read error parsing parameter name.\nEnsure file is"
             " formatted with lines like 'rr=0.30', exluding the single quotes, and no spaces anywhere.\nExiting.\n");
      exit(0);
    } // if

    valc = strtok(NULL, "\n");
    if (valc == NULL) {
      printf("File read error parsing parameter value.\nEnsure file is"
             " formatted with lines like 'rr=0.30', exluding the single quotes.\nExiting.\n");
      exit(0);
    }

    if (strcmp(param, "fps") == 0)
      bike->fps = atof(valc);
    else if (strcmp(param, "ti") == 0)
      bike->ti = atof(valc);
    else if (strcmp(param, "tf") == 0)
      bike->tf = atof(valc);
    else if (strcmp(param, "scale") == 0)
      bike->scale = atof(valc);
    else if (strcmp(param, "h") == 0)
      bike->h = atof(valc);
    else if (strcmp(param, "eps_abs") == 0)
      bike->eps_abs = atof(valc);
    else if (strcmp(param, "eps_rel") == 0)
      bike->eps_rel = atof(valc);
    else if (strcmp(param, "a_y") == 0)
      bike->a_y = atof(valc);
    else if (strcmp(param, "a_dydt") == 0)
      bike->a_dydt = atof(valc);
    else if (strcmp(param, "integrator") == 0) {
      if (strcmp(valc, "rk2") == 0)
        bike->ode_T = gsl_odeiv_step_rk2;
      else if (strcmp(valc, "rk4") == 0)
        bike->ode_T = gsl_odeiv_step_rk4;
      else if (strcmp(valc, "rkf45") == 0)
        bike->ode_T = gsl_odeiv_step_rkf45;
      else if (strcmp(valc, "rkck") == 0)
        bike->ode_T = gsl_odeiv_step_rkck;
      else if (strcmp(valc, "rk8pd") == 0)
        bike->ode_T = gsl_odeiv_step_rk8pd;
      else if (strcmp(valc, "rk2imp") == 0)
        bike->ode_T = gsl_odeiv_step_rk2imp;
      else if (strcmp(valc, "rk4imp") == 0)
        bike->ode_T = gsl_odeiv_step_rk4imp;
      else if (strcmp(valc, "gear1") == 0)
        bike->ode_T = gsl_odeiv_step_gear1;
      else if (strcmp(valc, "gear2") == 0)
        bike->ode_T = gsl_odeiv_step_gear2;
      gsl_odeiv_step_free(bike->ode_s);
      bike->ode_s = gsl_odeiv_step_alloc(bike->ode_T, 10);
    }
  } // for 

  fclose(fp);
} // readIntegrationParams()
*/