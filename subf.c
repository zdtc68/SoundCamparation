#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <lapacke.h>
#include <cblas.h>
#include <svd.h>
#include <matrixmul.h>
#include <xnorm.h>

float subf(int n,int t,int t1, float *input, float **output, float norm)
{
	int i,j;
	float sum=0,sum1=0,x;
	printf("%d,%d\n",t,t1);
	if(t<=t1)
		for(i=0;i<n;i++)
		{
			output[i][t-1]=input[i];
		}
	if(t==t1)
	{
		int nnn;
		float **u, **s, **v, **s1, **v1, **MMM;
		u=(float **)malloc(n*sizeof(float *));
		s=(float **)malloc(n*sizeof(float *));
		v=(float **)malloc(t1*sizeof(float *));
		for (i=0;i<n;i++)
		{
			u[i]=(float *)calloc(n,sizeof(float));
			s[i]=(float *)calloc(t1,sizeof(float));
		}
		for(i=0;i<t1;i++)
			v[i]=(float *)calloc(t1,sizeof(float));
		svd(n,t1,output,u,s,v);


		printf("the v matrix is=\n");
		nnn=t1;
		printf("%d\n",t1);
		s1=(float **)malloc(n*sizeof(float *));
		v1=(float **)malloc(nnn*sizeof(float *));
		MMM=(float **)malloc(n*sizeof(float *));
		for(i=0;i<n;i++)
		{
			s1[i]=(float *)calloc(nnn,sizeof(float));
			MMM[i]=(float *)calloc(nnn,sizeof(float));
		}
		for(i=0;i<nnn;i++)
			v1[i]=(float *)calloc(nnn,sizeof(float));
		for(i=0;i<n;i++)
			for(j=0;j<nnn;j++)
			{
				
				s1[i][j]=s[i][j];
			}
		for (i=0;i<nnn;i++)
			for(j=0;j<nnn;j++)
			{
				
	       			v1[i][j]=v[i][j];
			}
		matrixmul(n,n,nnn,u,s,MMM);
		matrixmul(n,nnn,nnn,MMM,v,output);

		for (i=0;i<nnn;i++)
			free(v1[i]);
		for(i=0;i<n;i++)
		{
			free(MMM[i]);
			free(s1[i]);
		}
		free(MMM);
		free(v1);
		free(s1);
		for(i=0;i<t1;i++)
			free(v[i]);
		for(i=0;i<n;i++)
		{
			free(s[i]);
			free(u[i]);
		}
		free(v);
		free(s);
		free(u);


	}
	if(t>t1)
	{
		for(i=0;i<t1;i++)
		{
			sum=xnorm(input,output,i,n);
			if((i==0 || sum<sum1) && sum>0)
				sum1=sum; 
		}
		norm=sum1;
	
	printf("norm=%e",norm);
	}




}


