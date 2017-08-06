#ifndef TWIDDLER_H
#define TWIDDLER_H

#include "PID.h"

class Twiddler {
 private:
  double dp;
  double di;
  double dd;

  double threshold;
  double best_error;

  int num_skip;
  int num_avg;
  int counter;

  double total_error;

  int state;

  PID& pid;

 public:

  Twiddler(int num_skip, int num_avg, double dp, double di, double dd, double threshold, PID& pid);
  
  void UpdateError(double cte);

  bool IsFinished();
  
};

#endif
