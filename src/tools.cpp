#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
  TODO:
    * Calculate the RMSE here.
  */
  VectorXd rmse(4);
  rmse << 0,0,0,0;
    
//#########################################

//Estimation vector = 0
  if (estimations.size() ==0){
      cout << "ERROR, estimation vector is empty" << endl;
      return rmse;
  }

//#########################################
  
//Same length
  if (estimations.size() != ground_truth.size()){
    cout << "ERROR, is diffetent length" << endl;
    return rmse;
  }
    
//#########################################
    
//Ground_truth == 0
  if (ground_truth.size() ==0){
    cout << "ERROR, ground_truth is empty" << endl;
    return rmse;
  }
    
//#########################################

//Residuals
  for(unsigned int i=0; i < estimations.size(); ++i){
    VectorXd diff = estimations[i] - ground_truth[i];

    diff = diff.array()*diff.array();
    rmse += diff;
  }
  
//#########################################
    
// Calculate the mean
  rmse = rmse / estimations.size();
  rmse = rmse.array().sqrt();
  return rmse;
}