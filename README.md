# Oversampling From Distortion

### 1)Why is oversampling needed? 
Nonlinear distortion (e.g., `tanh`) generate harmonics. Some of there exceed the Nyquist frequency (Fs/2) of the original sample rate. If you keep the original rate or downsample directly, thos out-of-band components fold back(alias) into the spectrum, producing spurious peaks.

<br>

### Solutionn Principle : 

Perform the nonlinear processing at a higher sample rate (where there's enough headroom), then remove the unnecessary high-frequency content, and finally bring the signal back down to the original sample rate.

~~~cpp
Input Signal 
     │
     ▼
[ Oversampling ]
     │
     ▼
[ Nonlinear Distortion ]
     │
     ▼
[ Downsampling ]
     │
     ▼
Output Signal
~~~
