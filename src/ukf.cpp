#include "ukf.h"
#include "Eigen/Dense"
#include <iostream>

using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::vector;

/**
 * Initializes Unscented Kalman filter
 * This is scaffolding, do not modify
 */
UKF::UKF() {
  // if this is false, laser measurements will be ignored (except during init)
  use_laser_ = true;

  // if this is false, radar measurements will be ignored (except during init)
  use_radar_ = true;

  // initial state vector
  x_ = VectorXd(5);

  // initial covariance matrix
  P_ = MatrixXd(5, 5);

  P_ << 1, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 1;
    
  // Process noise standard deviation longitudinal acceleration in m/s^2
  std_a_ = 30;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = 30;
  
  //DO NOT MODIFY measurement noise values below these are provided by the sensor manufacturer.
  // Laser measurement noise standard deviation position1 in m
  std_laspx_ = 0.15;

  // Laser measurement noise standard deviation position2 in m
  std_laspy_ = 0.15;

  // Radar measurement noise standard deviation radius in m
  std_radr_ = 0.3;

  // Radar measurement noise standard deviation angle in rad
  std_radphi_ = 0.03;

  // Radar measurement noise standard deviation radius change in m/s
  std_radrd_ = 0.3;
  //DO NOT MODIFY measurement noise values above these are provided by the sensor manufacturer.
  
/*-----------------------------------------------------*/
 
  ///* State dimension
  n_x_ = x_.size();
    
  ///* Augmented state dimension
  n_aug_ = n_x_ + 2;
    
  ///* Number of sigma points
  n_sig_ = 2 * n_aug_ +1;
    
  ///* Sigma point spreading parameter
  lambda_ = 3 - n_aug_;
    
  ///* Weights of sigma points
  weights_ = VectorXd(n_sig_);

  // Set the predicted sigma points matrix dimentions
  Xsig_pred_ = MatrixXd(n_x_, n_sig_);
  
  //Radar measurement noise covariance matrix

  R_radar_ = MatrixXd(3,3);
  R_radar_ << std_radr_*std_radr_, 0, 0,
              0, std_radphi_*std_radphi_, 0,
              0, 0,std_radrd_*std_radrd_;
    
  //Lidar measurement noise covariance matrix
  R_lidar_ = MatrixXd(2,2);
  R_lidar_ << std_laspx_*std_laspx_,0,
              0,std_laspy_*std_laspy_;

  /**
  TODO:

  Complete the initialization. See ukf.h for other member properties.

  Hint: one or more values initialized above might be wildly off...
  */
}
/*--------------------------------------------------------------------------------------------*/

UKF::~UKF() {}
/*
/**
 * @param {MeasurementPackage} meas_package The latest measurement data of
 * either radar or laser.
 */
void UKF::ProcessMeasurement(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Make sure you switch between lidar and radar
  measurements.
  */
    
//#############################################################################

  if ( !is_initialized_) {
    if (meas_package.sensor_type_ == MeasurementPackage::RADAR) {
        double rho = meas_package.raw_measurements_[0]; // range
        double phi = meas_package.raw_measurements_[1]; // bearing
        double rho_dot = meas_package.raw_measurements_[2]; // velocity of rh
        double x = rho * cos(phi);
        double y = rho * sin(phi);
        double vx = rho_dot * cos(phi);
        double vy = rho_dot * sin(phi);
        double v = sqrt(vx * vx + vy * vy);
        x_ << x, y, v, 0, 0;
    }
    
    else {
        x_ << meas_package.raw_measurements_[0], meas_package.raw_measurements_[1], 0, 0, 0;
    }
        
    // Saving first timestamp in seconds
    time_us_ = meas_package.timestamp_ ;
    // done initializing, no need to predict or update
    is_initialized_ = true;
    return;
  }
    
//#############################################################################

  // Calculate dt
  double dt = (meas_package.timestamp_ - time_us_)/1000000.0;
  time_us_ = meas_package.timestamp_;
  
  //Step
  Prediction(dt);
    
  if (meas_package.sensor_type_ == MeasurementPackage::RADAR && use_radar_) {
    UpdateRadar(meas_package);
  }
  if (meas_package.sensor_type_ == MeasurementPackage::LASER && use_laser_) {
    UpdateLidar(meas_package);
  }
}

/*---------------------------------------------------------------------------------------------

/**
 * Predicts sigma points, the state, and the state covariance matrix.
 * @param {double} delta_t the change in time (in seconds) between the last
 * measurement and this one.
 */
void UKF::Prediction(double delta_t) {
  /**
  TODO:

  Complete this function! Estimate the object's location. Modify the state
  vector, x_. Predict sigma points, the state, and the state covariance matrix.
  */
    
  //#############################################################################
  // Create sigma point matrix
  MatrixXd Xsig = MatrixXd(n_x_, 2 * n_x_ + 1);
    
  // Calc square root of P
  MatrixXd A = P_.llt().matrixL();
    
  // Set first col of sigma point matrix
  Xsig.col(0) = x_;
    
  // Set other sig points
  for (int i = 0; i < n_x_; i++)
  {
    Xsig.col(i+1)     = x_ + sqrt(lambda_+n_x_) * A.col(i);
    Xsig.col(i+1+n_x_) = x_ - sqrt(lambda_+n_x_) * A.col(i);
  }
    
    
}
/*---------------------------------------------------------------------------------------------

/**
 * Updates the state and the state covariance matrix using a laser measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateLidar(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Use lidar data to update the belief about the object's
  position. Modify the state vector, x_, and covariance, P_.

  You'll also need to calculate the lidar NIS.
  */
}
/*---------------------------------------------------------------------------------------------

/**
 * Updates the state and the state covariance matrix using a radar measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateRadar(MeasurementPackage meas_package) {
  /**
  TODO:

  Complete this function! Use radar data to update the belief about the object's
  position. Modify the state vector, x_, and covariance, P_.

  You'll also need to calculate the radar NIS.
  */
}
