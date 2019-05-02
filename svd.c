#include <stdio.h>
#include <math.h>
#include <cblas.h>
#include <lapacke.h>
#include <stdlib.h>
#include <unistd.h>
float svd(int x, int y, float **a, float **u, float **s, float **v)
{
	int i,j,z;
	if(x>y)
		z=y;
	else
		z=x;

	float *b,*S,*U,*V,*sup,t;
	b=(float *)malloc(x*y*sizeof(float));
	S=(float *)malloc(z*sizeof(float));
	U=(float *)malloc(x*x*sizeof(float));
	V=(float *)malloc(y*y*sizeof(float));
	sup=(float *)malloc((x-1)*sizeof(float));
	for(i=0;i<y*y;i++)
		V[i]=0.0f;
	for(j=0;i<x*x;i++)
		U[i]=0.0f;
	printf("\n");
	for(j=0;j<y;j++)
		for(i=0;i<x;i++)
		{
			b[i+j*x]=a[i][j];
		}
	printf("\n");
	printf("\n");
	for(i=0;i<z;i++)
		S[i]=0.0f;
	LAPACKE_sgesvd(LAPACK_COL_MAJOR,'A','A',x,y,b,x,S,U,x,V,y,sup);
	t=0.0f;
	for(i=0;i<x;i++)
		for(j=0;j<y;j++)
			if(i==j)
				s[i][j]=S[i];
			else
				s[i][j]=0.0f;
	for(i=0;i<x;i++)
		for(j=0;j<x;j++)
			u[i][j]=U[i+j*x];
	for(i=0;i<y;i++)
		for(j=0;j<y;j++)
			v[i][j]=V[i+j*y];

	free(sup);
	free(V);
	free(U);
	free(S);
	free(b);
}
