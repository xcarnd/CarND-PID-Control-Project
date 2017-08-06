#include <limits>
#include "Twiddler.h"
#include <iostream>

static const int P_UP = 0;
static const int P_DOWN = 1;
static const int I_UP = 2;
static const int I_DOWN = 3;
static const int D_UP = 4;
static const int D_DOWN = 5;

using namespace std;

Twiddler::Twiddler(int num_skip, int num_avg, double dp, double di, double dd, double threshold, PID& pid)
  : dp(dp),
    di(di),
    dd(dd),
    threshold(threshold),
    best_error(numeric_limits<double>::max()),
    num_skip(num_skip),
    num_avg(num_avg),
    counter(0),
    total_error(0.0),
    pid(pid) {
  state = P_UP;
  pid.Kp += dp;
  std::cout<<"Twiddler tuning Kp to: "<<pid.Kp<<std::endl;
}

void Twiddler::UpdateError(double cte) {
  if (IsFinished()) {
    return;
  }

  ++counter;

  if ((counter % 1000) == 0) {
    std::cout<<counter<<" "<<std::flush;
  }

  if (counter < num_skip) {
    if (counter == 1) {
      std::cout<<"Current parameters:"
	       <<" Kp = "<<pid.Kp
	       <<" Ki = "<<pid.Ki
	       <<" Kd = "<<pid.Kd
	       <<std::endl;
      std::cout<<"Waiting the PID controller to be stable."<<std::endl;
    }
    return;
  }

  if (counter == num_skip) {
    std::cout<<"Collecting error feedback."<<std::endl;
  }

  if (counter == num_avg + num_skip) {
    // get average error and compare with best_error
    double avg_error = total_error / num_avg;
    std::cout<<"Best error: "<<best_error<<", current error: "<<avg_error<<std::endl;
    if (avg_error < best_error) {
      best_error = avg_error;
      // retain the twiddled parameter, increase delta, and continue
      // with the next twiddle round (for the next pid parameter)
      if (state == P_UP || state == P_DOWN) {
	dp *= 1.1;
	state = I_UP;
	pid.Ki += di;
	std::cout<<"Twiddler tuning Ki to: "<<pid.Ki<<std::endl;
      } else if (state == I_UP || state == I_DOWN) {
	di *= 1.1;
	state = D_UP;
	pid.Kd += dd;
	std::cout<<"Twiddler tuning Kd to: "<<pid.Kd<<std::endl;
      } else if (state == D_UP || state == D_DOWN) {
	dd *= 1.1;
	state = P_UP;
	pid.Kp += dp;
	std::cout<<"Twiddler tuning Kp to: "<<pid.Kp<<std::endl;
      }
    } else {
      // failed getting a better error. try the other direction or reset the
      // parameter and reduce delta, then go for the next twiddle round.
      if (state == P_UP) {
	state = P_DOWN;
	pid.Kp -= (2 * dp);
	std::cout<<"Twiddler tuning Kp to: "<<pid.Kp<<std::endl;
      } else if (state == P_DOWN) {
	pid.Kp += dp;
	dp *= 0.9;
	state = I_UP;
	pid.Ki += di;
	std::cout<<"Twiddler tuning Ki to: "<<pid.Ki<<std::endl;
      } else if (state == I_UP) {
	state = I_DOWN;
	pid.Ki -= (2 * di);
	std::cout<<"Twiddler tuning Ki to: "<<pid.Ki<<std::endl;
      } else if (state == I_DOWN) {
	pid.Ki += di;
	di *= 0.9;
	state = D_UP;
	pid.Kd += dd;
	std::cout<<"Twiddler tuning Kd to: "<<pid.Kd<<std::endl;
      } else if (state == D_UP) {
	pid.Kd -= (2 * dd);
	state = D_DOWN;
	std::cout<<"Twiddler tuning Kd to: "<<pid.Kd<<std::endl;
      } else if (state == D_DOWN) {
	pid.Kd += dd;
	dd *= 0.9;
	state = P_UP;
	pid.Kp += dp;
	std::cout<<"Twiddler tuning Kp to: "<<pid.Kp<<std::endl;
      }
    }

    std::cout<<"Current parameter delta:"
	     <<" dP = "<<dp
	     <<" dI = "<<di
	     <<" dD = "<<dd
	     <<std::endl;
    
    // reset counter and error for the next twiddle round.
    counter = 0;
    total_error = 0.0;
    pid.total_cte = 0.0;
    pid.last_cte_initialized = false;
  } else {
    // use cte^2 for error.
    total_error += (cte * cte);
  }
  
}

bool Twiddler::IsFinished() {
  return (dp + di + dd) < threshold;
}
