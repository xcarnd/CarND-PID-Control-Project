# PID Control

这是我在提交项目并且项目通过以后，参考了导师的实现以及论坛上的一些经验，
重新尝试调整参数并审视PID控制器中各组分的效果，对之前的writeup.md做一些补充或者修正。

用中文写，避免词不达意……

## PID 控制器中各组分的效能分析

### P 组分

毫无疑问P依旧是用来控制避免偏差的主力，直接正比于当前的CTE给出对应的反馈，是最直观的控制组分。

P值的取值与速度有一定的关系。速度低的时候一般P值大点都没所谓，这是因为速度低时候转弯较慢，
太低的P值反而会让车辆反应迟钝。当速度变大，P值就不能太大了，否则一点点小弯就可能导致翻车。

这和我在writeup.md里面思考的还是一致的：在合适的范围里面，P值大一点，是有利于减轻overshoot的；
超过这个合适的范围以后，就会出现反效果。

### D 组分

D 组分是正比于负的偏差变化率。偏差变化越明显，D组分就会给出更明显的抵消这个变化的响应。
实际上D组分正是通过抵消变化率的方式去抵抗overshoot的。

在处理转向的时候可能感觉会更明显。转向的时候，CTE会首先逐渐变大，然后在P组分的控制下逐渐变小。
在逐渐变大的情况时，D组分为了抵消这种变化，会施加正向的反馈，和P组分一起修正偏差；但当CTE
开始变小的时候，D组分又会为了抵消变化，施加反向的反馈，相当于阻碍了偏差的修正，让偏差归0变慢，
有可能反而让overshoot的程度更大，虽然不会十分明显（我后来重新尝试参数的时候就感觉出现过这种
情况，在处理转弯的时候最容易出现）。

不过，让偏差归0变慢，从另一个角度说归零以后在反方向overshoot的程度相对来说就会降低。这样考虑
的话，虽然不能说D组分总能解决降低overshoot，但它确实能起到让系统尽快进入稳定状态的效果（当然，
需要建立在D组分取值合适的前提下。这个在writeup.md里面应该有个小视频可以说明问题了）。

### I 组分

I组分的分析应该没太大问题吧。暂时还没发现。

