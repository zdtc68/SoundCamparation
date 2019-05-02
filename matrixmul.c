#include<stdio.h>
#include<lapacke.h>
#include<math.h>
#include<cblas.h>

float matrixmul(int a, int b, int c, float **A, float **B, float **C)
{
	
	float *AAA,*BBB,*CCC;
	AAA=(float *)malloc(a*b*sizeof(float));
	BBB=(float *)malloc(b*c*sizeof(float));
	CCC=(float *)malloc(a*c*sizeof(float));
	int i,j;
	printf("\n");
	for(i=0;i<a;i++)
		for(j=0;j<b;j++)
			AAA[i+j*a]=A[i][j];
	for(i=0;i<b;i++)
		for(j=0;j<c;j++)
			BBB[i+j*b]=B[i][j];
	cblas_sgemm(CblasRowMajor,CblasTrans,CblasTrans,a,c,b,1.0f,AAA,a,BBB,b,0.0f,CCC,c);
	for(i=0;i<a;i++)
		for(j=0;j<c;j++)
			C[i][j]=CCC[i*c+j];
	free(AAA);
	free(BBB);
	free(CCC);
}
