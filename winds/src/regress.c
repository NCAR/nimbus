/* regress.c -- do regressions
*/
#include <math.h>


/****************************  WINDS INCLUDES  ******************************/

#include <constants.h>
#include <lengths.h>
#include <calib.h>

/* Find the least squares fit */

leastsq(volt,point,iorder,npts,detn,det,cals)
double volt[MAXSIZE];	/* readings from the instrument */
int iorder;		/* order of the cal equation */
int npts;		/* number of values sampled */
float *detn;		/* normalized determinant */
float *det;		/* determinant */
float cals[MAXSIZE];	/* the cal coeffs found by least sq fit */
float point[MAXSIZE];	/* value at which instrument was calibrated */
{
int i,j;
float aflt;
float a[MAXSIZE][MAXSIZE];
float b[MAXSIZE][MAXSIZE];
float c[MAXSIZE][MAXSIZE];

/* set up the a matrix */
for (j=0;j<npts;j++)
  {
  aflt = 1.;
  for(i=0;i<iorder;i++)
	{
	a[j][i] = aflt;
	aflt = aflt*volt[j];
	}
  }
trans(a,b,npts,iorder);		/* B is A transpose */
mult(b,a,c,iorder,npts,iorder);	/* C is A transpose times A (At*A)*/
invdet(c,iorder,detn,det);	/* invert C to get inverse of At*A */
mult(c,b,a,iorder,iorder,npts);	/* inverse of At*A times At */
for(i=0;i<npts;i++) b[i][0] = point[i];
mult(a,b,c,iorder,npts,1);  /* get the cal coefficients */
for(i=0;i<iorder;i++) cals[i] = c[i][0];
}
/* This section contains
	invdet - a matrix inversion routine which returns determinants
	mult   - a matrix multiplication routine
	trans  - produces the transpose of a matrix
*/

/* Matrix inversion using Gauss-Jordan elimination with column shifting
to maximize pivot elements.  From "Numerical Methods" by R.W. Hornbeck (1975)
*/

invdet(c,n,dtnrm,detm)
float c[MAXSIZE][MAXSIZE];	/* array to be inverted */
int n;			/* row and column dimension of array to invert */
float *dtnrm;		/*normalized determinant. Very small value => error*/
float *detm;		/* Magnitude of the determinant */

{
int j[MAXSIZE+21];	/* must be at least 21 greater than either size of c*/
int k,l,m;
float pd,dd,cc,s;

pd = 1.;
for (l=0;l<n;l++)
  {
   dd = 0.;
   for (k=0;k<n;k++) dd += c[l][k]*c[l][k];
   dd = sqrt((double)dd);
   pd = pd*dd;
  }
for (l=0;l<n;l++) j[l+20] = l;
*detm = 1.;
for (l=0;l<n;l++)
  {
   cc = 0.;
   m = l;
   for (k=l;k<n;k++)
	{
	if(((cc<0?-cc:cc) - (c[l][k]<0?-c[l][k]:c[l][k]))>=0.) continue;
	m = k;
	cc = c[l][k];
	}
   if(l!=m)
	{
	k = j[m+20];
	j[m+20] = j[l+20];
	j[l+20] = k;
	for (k=0;k<n;k++)
		{
		s = c[k][l];
		c[k][l] = c[k][m];
		c[k][m] = s;
		}
	}
   c[l][l] = 1.;
   *detm = *detm*cc;
   for (m=0;m<n;m++) c[l][m] = c[l][m]/cc;
   for (m=0;m<n;m++)
	{
	if(l==m) continue;
	cc = c[m][l];
	if(cc == 0.) continue;
	c[m][l] = 0.;
	for (k=0;k<n;k++) c[m][k] -= cc*c[l][k];
	}
   }
for(l=0;l<n;l++)
  {
   if(j[l+20]==l) continue;
   m = l;
   do
	{
	m++;
	if(j[m+20]==l) break;
	} while(n>m);
   j[m+20] = j[l+20];
   for(k=0;k<n;k++)
	{
	cc = c[l][k];
	c[l][k] = c[m][k];
	c[m][k] = cc;
	}
   j[l+20] = l;
  }
if(*detm<0)*detm = -*detm;
*dtnrm = *detm/pd;
}

/* Multiply two matrices - return a third */
/* If this routine is used to multiply a [mxn][nx1] it won't work.
   C varies the column first and then the row.  This routine expects to vary
   the row number of the second array and will pick up garbage unless the 
   [nx1] is forced into the expected locations in a two dimensional array.
*/
mult(amat,bmat,cmat,nra,nca,ncb)
float amat[MAXSIZE][MAXSIZE];	/* input matrix of nra rows,nca columns */
float bmat[MAXSIZE][MAXSIZE];	/* input matrix of ncb columns */
float cmat[MAXSIZE][MAXSIZE];	/* C = A*B, C matrix is nra rows, ncb columns */
int nra;		/* number of rows of A matrix */
int nca;		/* number of columns of A matrix */
int ncb;		/* number of columns of B matrix */
{
int i,j,k;

for (i=0;i<nra;i++)
  {
   for(j=0;j<ncb;j++)
	{
	cmat[i][j] = 0.;
	for(k=0;k<nca;k++) cmat[i][j] += amat[i][k]*bmat[k][j];
	}
  }
}

/* Find the transpose of a matrix */
trans(amat,tmat,nra,nca)
float amat[MAXSIZE][MAXSIZE];	/* input matrix of nra rows,nca columns */
float tmat[MAXSIZE][MAXSIZE];	/* A transpose (At) */
int nra;		/* number of rows in A */
int nca;		/* number of columns in A */
{
int i,j;

for (i=0;i<nra;i++)
   {
    for(j=0;j<nca;j++) tmat[j][i] = amat[i][j];
   }
}

/* Find the correlation coefficients */
/* Calculates the difference at each point in engineering units */

coeff(x,iordr,x1,y,npts,r,erms,emax)
int iordr;              /* order of equation */
float x[MAXSIZE];	/* contains iordr cal coefficients */
double x1[MAXSIZE];	/* contains npts voltage readings */
float y[MAXSIZE];	/* contains npts of desired values */
float *r;		/* the correlation coefficient */
float *erms;		/* rms error */
float *emax;		/* maximum error */
{
float diff[MAXSIZE];	/* contains desired values(y)-calibrated values(x1*x)*/
float y1_[MAXSIZE];
int i,j;
float errsq,sumy=0.,sumx=0.,sumys=0.,sumxs=0.,sumxy=0.,sxx,syy,sxy;
float aflt;

/* Find the resulting output using given cal coeffs */
*erms = 0.0;
*emax = 0.0;
for (i=0;i<npts;i++)
  {
   y1_[i] = 0.;
   aflt = 1.0;
   for (j=0;j<iordr;j++)
	{
	y1_[i] += x[j]*aflt;
	aflt = aflt*x1[i];
	}
   diff[i] = y[i] - y1_[i];
   errsq = diff[i]*diff[i];
   *erms += errsq;
   if(errsq>*emax) *emax = errsq;
  }

/*  max error and root mean squared error */
*emax = sqrt(*emax);
*erms = sqrt(*erms/npts);

/* Correlation coefficient */
for(i=0;i<npts;i++)
   {
    sumx += y1_[i];
    sumy += y[i];
    sumxs += y1_[i]*y1_[i];
    sumys += y[i]*y[i];
    sumxy += y1_[i]*y[i];
   }
sxx = npts*sumxs - sumx*sumx;
syy = npts*sumys - sumy*sumy;
sxy = npts*sumxy - sumx*sumy;
*r = sxy/sqrt(sxx*syy);
}
