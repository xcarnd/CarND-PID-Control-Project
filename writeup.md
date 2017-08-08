# PID Controller

[//]: # (Image References)
[p-only]: ./figures/P-only.png "Comparision with different Kp."
[pd]: ./figures/PD.png "Comparison with different Kd."
[pd-too-big-d]: ./figures/PD-too-big-D.mp4 "When D getting too big."
[p-pd-pid]: ./figures/P-PD-PID.png "Comparing P, PD and PID controller."
[cte2-mean]: ./figures/CTE2-Mean.png "Comparing P, PD and PID controller - Mean of CTE square." 

## Reflection

### 1. Describe the effect each of the P, I, D components had in your implementation.

 The final PID controller hypermeters I picked up was:

- Kp = 1.96045
- Ki = 0.00966393
- Kd = 18.4001

And I'll go through each of them.

#### P component
The P component is the main "force" to make the vehicle back to the track when it is off-track. The bigger the P component is, the faster the vehicle will react to CTE. 

It may be counter-intuitive that bigger P component will make overshoot less obvious. In fact, small P component gives weak response, it will also react slowly to overshooting too, making overshooting more obvious.

But it does not means the P component can be immoderately big because the output is saturated at 1/-1. If P component is too big, even a tiny change of CTE will make the controller yielding as huge response as the system can be, which will make the system unstable.

Below's a figure for P-only controller CTE-Updates graph with different Kp.

![Comparison of different Kp][p-only]

#### D component
The D component gives response proportional (in the other direction) to the change rate of CTE. This component is helpful to reduce overshooting because the more obvious overshooting is, the larger the CTE change rate, and the larger the D component "negate" the change rate. With D component, the PID controller will get "stable" faster.

Larger D component will usually make overshooting less obvious. For the same reason, D component shall not be too large, otherwise it will push the system into an unstable state.

Below's a figure for PD controller CTE-Updates graph with different Kd.

![Comparison of different Kd][pd] 

For overkilling D component, it will affect the system in another manner. Here's a video clip for D component getter too large. (5x the optimal Kd)

![When D component is too large][pd-too-big-d]

The vehicle is staying on the track, but the steering angle keeps oscillating crazily.  

#### I component
The I component is used for tackling with system bias. It will give stronger response as the accumulated CTE grows.

The I component is the most sensitive component. It gives response proportional to the accumulated CTE, which will grow very fast. Therefore it is unwise to choose an I component with the same magnitude of the P & D components.

Because I component gives an additional positive response, overshooting will be slightly more obvious than PD controller. Below's a figure comparing P controller, PD controller and PID controller used in this project.

![Comparison of P, PD and PID controller][p-pd-pid]

However I think there is no system bias in this project, or at least negligible. So in theory, Ki = 0 shall give good enough control. Why twiddle will pick up Ki > 0? I think that's because there're turns in the track. When vehicle is driving across these turns, accumulated CTE grows, and the I component can give the system an extra response to steer back. Below's a figure comparing the mean square error for P controller, PD controller and PID controller.  (Log scale is used for y axis)

![Comparison of P, PD and PID controller - Mean of CTE square][cte2-mean]

### 2. Describe how the final hyperparameters were chosen.

Hyperparameters were chosen by both manually tuning and (partial) twiddle.

The initial Kp, Ki, Kd for PID controller and Dp, Di, Dd for twiddle were carefully picked in order to make the vehicle not flipping off the track, otherwise the vehicle would fail to complete the track and would render twiddle useless.

The speed the vehicle was driving at was manually picked too. The speed was independent with the steering angle, so I've tried several speed setting and check whether the car could drive without touching the yellow line. The final speed I chose was 15 mph.

Codes for twiddle implementation can be found in `Twiddler.cpp`. My implementation basically followed the one presented in the lessons. Changes were make in order to make twiddle fitting the project. Mainly, to perform twiddle I have to return some metrics to gauge at some point, then change the parameters a little bit and continue the twiddle loop.

To get a meaningful metric, I decided to calculate the mean of square errors of running along the track for one full lap. Since no information were offered by the simulator for whether one full lap was past, I used the number of updates as the criteria. On my machine with the target speed I was used, ~9500 updates would be taken for one full lap. The first 500 updates were ignored to let the PID controller be stable with the new parameters once the previous twiddle round was completed and the next round was set. 

Although workable, my twiddle implementation was time consuming. It might took 2+ days to narrowing the hyperparameter's delta into a small enough value. In fact, I had stopped twiddle when it found some parameter set I thought was good enough.
