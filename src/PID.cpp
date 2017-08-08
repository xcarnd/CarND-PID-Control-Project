#include "PID.h"

PID::PID()
  : p_error(0.0),
    i_error(0.0),
    d_error(0.0),
    current_cte(0.0),
    last_cte(0.0),
    last_cte_initialized(false),
    total_cte(0.0),
    Kp(0.0),
    Ki(0.0),
    Kd(0.0)
{}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
}

void PID::UpdateError(double cte) {
  if (!last_cte_initialized) {
    current_cte = cte;
    last_cte = current_cte;
    last_cte_initialized = true;
  } else {
    last_cte = current_cte;
    current_cte = cte;
  }
  total_cte += cte;

  p_error = cte;
  i_error = total_cte;
  d_error = cte - last_cte;
  
}

double PID::GetInputValue(double min, double max) {
  double val = -(Kp * p_error + Ki * i_error + Kd * d_error);
  if (val > max) {
    val = max;
  }
  if (val < min) {
    val = min;
  }
  return val;
}

double PID::TotalError() {
  return total_cte;
}

