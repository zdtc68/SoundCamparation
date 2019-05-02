#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fftw3.h>
#include "spidriver_host.h"
#include "adcdriver_host.h"
#include <svd.h>
#include <matrixmul.h>
#include <cblas.h>
#include <lapacke.h>
#include <subf.h>
#define N 256



//-----------------------------------------------------
// Called when Ctrl+C is pressed - triggers the program to stop.
void stopHandler(int sig) {
  adc_quit();
  exit(0);
}


//==========================================================
// This is main program which exercises the A/D.  You may use
// this program as a starting place for development of your
// own code.
int main (void)
{
  // Loop variables
  uint32_t i;
  uint32_t j;

  // Buffers for tx and rx data from A/D registers.
  ////uint32_t tx_buf[3];
  ////uint32_t rx_buf[4];

  // Measured voltages from A/D
  float volt;          // Single measurement.
  float volts[1024];   // Vector of measurements 

  // File ID
  ////FILE *fp;

  // Used in peak-peak measurement
  float vmax, vmin;

  // Stuff used with "hit return when ready..." 
  char dummy[8];

  printf("------------   Starting main.....   -------------\n");

  // Run until Ctrl+C pressed:
  signal(SIGINT, stopHandler);

  // Sanity check user.
  if(getuid()!=0){
     printf("You must run this program as root. Exiting.\n");
     exit(EXIT_FAILURE);
  }

  // Initialize A/D converter
  adc_config();

  
  // Now stay in this loop forever.
  j=256;
  printf("--------------------------------------------------\n");
  printf("Now take the folowing data as basis\n");
  printf("Hit return when ready -->\n");
  fgets (dummy, 8, stdin);
  adc_set_samplerate(SAMP_RATE_31250);
  int basisn=7,t=1;
  float basis[j][basisn];
 
  while(1)
  {
    adc_read_multiple(j, volts);
   printf("Values read = \n");
    int m,n,a[j],b[j],x1=0,y1=0,judge=1;
    float diff=0, *x;
    x=(float *)calloc(j,sizeof(float));
    fftw_complex *in=NULL;
    fftw_complex *out=NULL;
    in=(fftw_complex *)malloc(j*sizeof(fftw_complex));
    out=(fftw_complex *)malloc(j*sizeof(fftw_complex));
    for(m=0;m<j;m++)
    {
	    in[m][0]=volts[m];
	    in[m][1]=0;
    }
    fftw_plan p;
    p=fftw_plan_dft_1d(j,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_cleanup();

    for (m=0;m<j;m++)
    {
	    a[m]=(float)(out[m][0]);
	    b[m]=(float)(out[m][1]);
	    x[m]=0.2*sqrt(a[m]*a[m]+b[m]*b[m]);
	    if (x[m]>50)x[m]=50;
	    if (t<=basisn) basis[m][t]=x[m];
	    if (x[m]>x1)
	    {
		    x1=x[m];
		    y1=m;
	    }
    }
    for (m=40;m>=0;m--)
	    {
		    if (m==0)printf("+");
		    else printf("|");
		    for (n=0;n<j;n=n+2)
		    {
			    if (m==0)
			    {
				    if((int)x[n]==0) printf("*");
				    else printf("-");
			    }
			    else if ((int)x[n]==m) printf("*");
			    else printf(" ");
		    }
		    if (m==0)printf("time");
		    printf("\n");
	    }
    free(out);
    free(in);
    float norm;
    float **basiss;
    basiss=(float **)calloc(j,sizeof(float *));
    for(i=0;i<j;i++)
	    basiss[i]=(float *)calloc(basisn,sizeof(float));
    if(1)
	    for(int i1=0;i1<j;i1++)
		    for(int i2=0;i2<basisn;i2++)
			    basiss[i1][i2]=basis[i1][i2];
    float normx=0;
    for(i=0;i<j;i++)
	    normx=normx+x[i]*x[i];
    printf("the norm of input data=%.5f\n",normx);
	subf(j,t,basisn,x,basiss,norm);
    if(t<=basisn)
    	for (i=0;i<j;i++)
	    	basis[i][t-1]=basiss[i][t-1];
    printf("\n");
   



 
    if (t==basisn)
    {
	    printf("basis constructed, now start comparison\n");
	    printf("hit enter when ready to compare\n");
	    fgets (dummy,8,stdin);
    }
    usleep(1000000);
    t=t+1;
    for (i=0;i<j;i++)
	   free(basiss[i]);
    free(basiss);
    free(x);

  }


}

