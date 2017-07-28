#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  double current_cte;
  double last_cte;
  bool   last_cte_initialized;
  double total_cte;
  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
   * Calculate suitable steering angle given the cte collected so far
   */
  double GetSteeringAngle();

  /*
   * Calculate suitable throttle given the current speed and cte collected so far
   */
  double GetThrottle(double speed);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
