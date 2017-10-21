/*   This is a simple Anti-lock Braking System. The inputs are the
//   number of ticks counted within 100 ms at each wheel. Outputs are
//   control signals for pressure control (0 - constant pressure, 2 -
//   increase pressure, -1 - decrease pressure).
*/

#include <systemc.h>
#include "settings.h"
#include "absasr.h"

void absasr::read_vr(){
  v_last[0] = v[0];
  v[0] = bus_vr.read();
#ifdef DEBUG
  cout << sc_time_stamp() << " speed at right front wheel: "<< v[0] << endl;
#endif
  v[0] = v[0] * VELOFACTOR;
  a[0] = (v[0] - tmp[0]);
  tmp[0] = v[0];
}

void absasr::read_vl(){
  v_last[1] = v[1];
  v[1] = bus_vl.read();
#ifdef DEBUG
  cout << sc_time_stamp() << " speed at left front wheel: "<< v[1] << endl;
#endif
  v[1] = VELOFACTOR * v[1];
  a[1] = (v[1] - tmp[1]);
  tmp[1] = v[1];
}

void absasr::read_hr(){
  v_last[2] = v[1];
  v[2] = bus_hr.read();
#ifdef DEBUG
  cout << sc_time_stamp() << " speed at right rear wheel: "<< v[2] << endl;
#endif
  v[2] = VELOFACTOR * v[2];
  a[2] = (v[2] - tmp[2]);
  tmp[2] = v[2];
}

void absasr::read_hl(){
  v_last[3] = v[3];
  v[3] = bus_hl.read();
#ifdef DEBUG
  cout << sc_time_stamp() << " speed at left rear wheel: "<< v[3] << endl;
#endif
  v[3] = VELOFACTOR * v[3];
  a[3] = (v[3] - tmp[3]);
  tmp[3] = v[3];
}


void absasr::ABS(){
  // ABS is only active if speed is higher than 6 kmh
  if(fv > ABSACTIVE){
    for(int i = 0; i < 4; i = i + 1){

	// compute slack
	lambda[i] = ((fv-v[i])*100)/fv;
#ifdef DEBUG
	cout << sc_time_stamp() << " lambda[" << i << "]: " << lambda[i] << endl;
	cout << sc_time_stamp() << " v[" << i << "]: " << v[i] << endl;
	cout << sc_time_stamp() << " a[" << i << "]: " << a[i] << endl;
#endif
	// control cycle as specified by BOSCH
  switch(s[i]){
    case 1:
      if (a[i] < minus_a){
#ifdef DEBUG
	    cout << sc_time_stamp() << " regulation at wheel " << i << endl;
#endif
	    p[i] = 0;
	    s[i] = 2;
	  }
	  break;
    case 2:
         if (lambda[i] > lambda_abs){
	   p[i] = -1;
	   s[i] = 3;
	 }
	 break;
       case 3:
         if (a[i] > minus_a){
	   p[i] = 0;
	   s[i] = 4;
	 }
	 break;
       case 4:
	 if (a[i] > plus_A){
	   p[i] = 2;
	   s[i] = 5;
	 }
         else if (a[i] < plus_a){
	   p[i] = 1;
	   s[i] = 1;
	 }
	 break;
       case 5:
	 if (a[i] > plus_A){
	   p[i] = 0;
	   s[i] = 4;
	 }
	 break;
       }

      }
#ifdef DEBUG
      cout << sc_time_stamp() << " ABS : pressure is " << p[0] << ", " << p[1] << ", " << p[2] << ", " << p[3] << endl;
#endif
      pr_vr.write(p[0]);
      pr_vl.write(p[1]);
      pr_hr.write(p[2]);
      pr_hl.write(p[3]);
    }
}

  // ASR control - main routine
void absasr::ASR(){

    int j = 3;

    // for each wheel
    for(int i = 0; i < 2; i = i + 1){

      if(v[i] > 0){

	// compute slip
	lambda[i] = ((v[i]-v[j])*100)/v[i];

#ifdef DEBUG
	cout << sc_time_stamp() << " ASR: lambda[" << i << "] is " << lambda[i] << endl;
#endif

	// slip exceeds given limit
	if(lambda[i] > lambda_asr){
	  if(a[0] > 0){
	    // increase pressure
	    p[i] = 2;
	  }
	  else{
	    // constant pressure
	    p[i] = 0;
	  }
	}
	else
	  {
	    // decrease pressure
	    p[i] = -1;
	  }
      }
      j = 2;
    }

#ifdef DEBUG
    cout << sc_time_stamp() << " ASR : pressure is " << p[0] << ", " << p[1] << ", " << p[2] << ", " << p[3] << endl;
#endif

    pr_vr.write(p[0]);
    pr_vl.write(p[1]);
}

int absasr::abs(int val){
      if (val >= 0)
	return val;
      else
	return -val;
}

  // main thread, triggers ASR or ABS control if necessary
void absasr::run(){
  int i;
  // INIT
  for(i = 0; i < 4; i=i+1){
	s[i] = 1;
	v[i] = 0;
	a[i] = 0;
	p[i] = 0;
	tmp[i] = 0;
  }

  while(true){
	wait(1, SC_MS);

	// read ticks from all wheels
	read_vl();
	read_hl();
	read_vr();
	read_hr();

	// find wheel with lowest acceleration, for car speed estimation
	i = 0;
	if(abs(a[1]) < abs(a[i]))
	  i = 1;
	if(abs(a[2]) < abs(a[i]))
	  i = 2;
	if(abs(a[3]) < abs(a[i]))
	  i = 3;

	temp_fv = v[i];
	fa = temp_fv - fv;

#ifdef DEBUG
	cout << endl;
	cout << sc_time_stamp() << " current Speed: " << temp_fv << endl;
	cout << sc_time_stamp() << " v[0]: " << v[0] <<  ", v[1]: " << v[1] << ", v[2]: " << v[2] << ", v[3]: " << v[3] << endl;
	cout << sc_time_stamp() << " current Acceleration: " << fa << endl;
	cout << sc_time_stamp() << " a[0]: " << a[0] <<  ", a[1]: " << a[1] << ", a[2]: " << a[2] << ", a[3]: " << a[3] << endl;
#endif

    if( fa < 0 ){
      if(fa < minus_a){
        fv=fv+AREF;
#ifdef DEBUG
	    cout << sc_time_stamp() << " all wheels are locked! " << endl;
	    cout << sc_time_stamp() << " estimated speed: " << fv << endl;
#endif
      }
      else
        fv = temp_fv;

      ABS();

    }
    else if (fa > 0){
	  fv = temp_fv;
	  ASR();
    }
  }
}
