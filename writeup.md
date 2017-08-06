# PID Controller

## Reflection

### 1. Describe the effect each of the P, I, D components had in your implementation.

The P component is the main "force" to make the vehicle back to the track when it is off-track. The bigger the P component is, the faster the vehicle will react to CTE. On the other hand, larger P component will make the vehicle overshoot more obviously. 

The I component is used for tackling with system bias. It will give stronger response as the accumulated CTE grows. In this project, system bias is not obvious (or no system bias if my observation is right) so this component play a less important role. In this project however, I think the I component can make the vehicle react more promptly to turns, because when making turns, the accumulated CTE will grow obviously, and the I component will give an extra steering output in these cases, making the vehicle steer more hardly.

The D component gives response proportional (in the other direction) to the change rate of CTE. This component is helpful to reduce overshooting because the more obvious overshooting is, the larger the CTE change rate, and the larger the D component "negate" the change rate. With D component, the PID controller will get "stable" faster. 

All of the three components cannot be too larger since the steering angle will be saturated at -1 or 1. If one of these parameters is getting too large, the steering angle will be set to its max value (25 or -25) most of the time. The vehicle will drive crazily and will easily go out of the track.

In my implementation of PID controller, when the car was driving across a sharp turn, it suddenly oscillated, nearly touching the yellow line of the track edge. I think that was because the steering angle is not only depend on CTE. In real world, drivers will keep steering even if the car is staying at the center of the road perfectly.

### 2. Describe how the final hyperparameters were chosen.

Hyperparameters were chosen by both manually tuning and twiddle.

The initial Kp, Ki, Kd for PID controller and Dp, Di, Dd for twiddle were carefully picked in order to make the vehicle not flipping off the track, otherwise the vehicle would fail to complete the track and would render twiddle useless.

The speed the vehicle was driving at was manually picked too. The speed was independent with the steering angle, so I've tried several speed setting and check whether the car could drive without touching the yellow line. The final speed I chose was 15 mph.

Codes for twiddle implementation can be found in `Twiddler.cpp`. My implementation basically followed the one presented in the lessons. Changes were make in order to make twiddle fitting the project. Mainly, to perform twiddle I have to return some metrics to gauge at some point, then change the parameters a little bit and continue the twiddle loop.

To get a meaningful metric, I decided to calculate the mean of square errors of running along the track for one full lap. Since no information were offered by the simulator for whether one full lap was past, I used the number of updates as the criteria. On my machine with the target speed I was used, ~9500 updates would be taken for one full lap. The first 500 updates were ignored to let the PID controller be stable with the new parameters once the previous twiddle round was completed and the next round was set. 

 