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

<br>

### Upsampling Implementation
"Zero insertion + interpolation(low-pass) filter"
The Interpolation filter's cutoff should be placed at the Nyquist frequency of the input signal 

<img width="850" height="292" alt="image" src="https://github.com/user-attachments/assets/c5ec8cc7-c182-4bf0-a079-0e08e31941e0" />


<br>


The graphs below illustrate the interpolation caused by the low-pass filter. Linear interpolation uses two neighboring samples to compute all in-between samples.

<img width="833" height="383" alt="image" src="https://github.com/user-attachments/assets/03484b30-2ee8-496f-b933-6db4ebf4cd78" />


<img width="833" height="383" alt="image" src="https://github.com/user-attachments/assets/b19f3838-2823-4a17-a649-06d61f6dec52" />




<br>
<br>
<br>


### Downsampling Implementation
Pre-aliasing(low-pass) filter first, and then discatd the samples. The cutoff frequency of the low-pass filter is the half of the target sample rate. 


<img width="752" height="264" alt="image" src="https://github.com/user-attachments/assets/7b8d1dfa-d8b9-44b1-bae3-57e4db8762d9" />


As shown in the below graph, if we downsample without passing through a pre-aliasing fiilter, the Nyquist frequency becomes lower, which can cause aliasing


<img width="757" height="456" alt="image" src="https://github.com/user-attachments/assets/caae6d2b-2373-470a-8270-1f1d5aa4a91f" />




