#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float xnorm(float *input1, float **input2,int m, int n)
{
	int i=0;
	float sum1=0.0 , sum2=0.0, norm=0.0,t;
	for (i=0;i<n;i++)
	{
		sum1=sum1+input1[i]*input1[i];
		sum2=sum2+input2[i][m]*input2[i][m];
	}
	if(abs(sum1)<0.0001)
		sum1=0.0001;
	if(abs(sum2)<0.0001)
		sum2=0.0001;
	for (i=0;i<n;i++)
	{
		t=input1[i]/sum1-input2[i][m]/sum2;
		norm=norm+t*t;
	}
	printf("\n");
	return norm;
}

