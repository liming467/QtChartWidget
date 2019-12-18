Fs=1000;
T=1/Fs;
L=1000;
newL=1024;    
t=0:newL-1;
t=t*T;
x=cos(2*pi*50*t);
x(:,L+1:newL)=0.0;
y=fft(x);
p1=abs(y/newL);
p2=p1(:,1:newL/2+1);
p2(:,2:end-1)=2*p2(:,2:end-1);
f=(0:newL/2)*Fs/newL;
plot(f(1:newL/2+1),p2(1:newL/2+1));

    