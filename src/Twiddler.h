#ifndef TWIDDLER_H
#define TWIDDLER_H

#include "PID.h"
#include <limits>

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

  bool roundEnd;

  PID& pid;

 public:

  Twiddler(int num_skip, int num_avg, double dp, double di, double dd, double threshold, PID& pid, double best_cte = std::numeric_limits<double>::max());
  
  void UpdateError(double cte);

  bool IsFinished();

  bool IsRoundEnd(bool reset);
  
};

#endif
