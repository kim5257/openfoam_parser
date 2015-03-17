void CG(double *A, double *B, double *X,int N)
{
	int i,j,n=0;
	double alpha,rt=0,e=0;
	double beta,es=0.00001,sum=0;
	double pt=0;


	double *x_temp	=(double *)calloc(sizeof(double),N);
	double *r		=(double *)calloc(sizeof(double),N);
	double *r_temp	=(double *)calloc(sizeof(double),N);
	double *p		=(double *)calloc(sizeof(double),N);
	double *p1		=(double *)calloc(sizeof(double),N);




	for(i=0;i<N;i++)  //////////////////  Initial x matrix (x0) 임의로 결정
	{
		X[i]=x_temp[i]=1;
	}

	for(j=0;j<N;j++)
	{
		sum=0;
		for(i=0;i<N;i++)
		{
			sum+=A[i+j*N]*X[i];
		}
		r[j]=B[j]-sum;  /////////////  STEP 1
		p[j]=r[j];
	}

	do  //////////////////////////  STEP 2
	{
		//////////////////////////  STEP 3
		pt=rt=e=0;
		for(i=0;i<N;i++)
		{
			p1[i]=0;
			for(j=0;j<N;j++)
			{
				p1[i]+=A[i+j*N]*p[j];  
			}
		}
		for(i=0;i<N;i++)
		{
			pt+=p1[i]*p[i];
			rt+=r[i]*r[i];		
		}
		alpha=rt/pt;
		////////////////////////////////////

		/////////////////////////// STEP 4
		for(i=0;i<N;i++)
		{
			x_temp[i]+=alpha*p[i];
			e+=fabs(x_temp[i]-X[i]);
		}
		////////////////////////////////////


		rt=0; pt=0;		
		for(i=0;i<N;i++)
		{
			r_temp[i]=0;
			for(j=0;j<N;j++)
			{
				r_temp[i]-=alpha*A[i+j*N]*p[j];  ///////////////  STEP 5
			}
			r_temp[i]+=r[i];
			rt+=r_temp[i]*r_temp[i];
			pt+=r[i]*r[i];
		}

		beta=rt/pt;  ////////////////////////////////////////////  STEP 7
		/////////////////////////////////////////////////  STEP 8
		for(i=0;i<N;i++)
		{
			p[i]=r_temp[i]+beta*p[i];
			r[i]=r_temp[i];
			X[i]=x_temp[i];
		}
		
	}while(e>es);

	free(x_temp);
	free(r);
	free(r_temp);
	free(p);
	free(p1);

}

void CG_sparse(double *A, double *B, double *X,int N)
{
	int i,j,n=0,NN,k=0;
	double alpha,rt=0,e=0;
	double beta,es=0.00001,sum=0;
	double pt=0;
	int count=0;

	double *x_temp	=(double *)calloc(sizeof(double),N);
	double *r		=(double *)calloc(sizeof(double),N);
	double *r_temp	=(double *)calloc(sizeof(double),N);
	double *p		=(double *)calloc(sizeof(double),N);
	double *p1		=(double *)calloc(sizeof(double),N);




	///////////////////////////  length of Sparse matrix  ///////////////////////
	for(i=0;i<N*N;i++)
	{
		if(A[i]!=0.0)
		{
			count++;
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////  Sparse Matrix  and  Interger matrix  //////////////////	
	double *At=(double *)calloc(sizeof(double),count);
	int    *Ar=(int    *)calloc(sizeof(int)   ,count);

	count=0;
	for(i=0;i<N*N;i++)
	{
		if(A[i]!=0.0)
		{
			At[count]=A[i];
			Ar[count]=i;
			count++;
		}
	}
	////////////////////////////////////////////////////////////////////////////////




	for(i=0;i<N;i++)  //////////////////  Initial x matrix (x0) 임의로 결정
	{
		X[i]=x_temp[i]=1;
	}

	k=0;
	sum=0;
	for(k=0;k<count;k++)
	{
		i=Ar[k]%N;
		j=Ar[k]/N;
		r[j]+=At[k]*X[i];
	}
	for(j=0;j<N;j++)
	{

		r[j]=B[j]-r[j];  /////////////  STEP 1
		p[j]=r[j];
	}

	do  //////////////////////////  STEP 2
	{
		//////////////////////////  STEP 3
		pt=rt=e=k=0;

		//////////////  Initialize  /////////////////
		for(j=0;j<N;j++)
		{
			p1[j]=r_temp[j]=0;
		}


		for(k=0;k<count;k++)
		{
			i=Ar[k]%N;
			j=Ar[k]/N;
			p1[j]+=At[k]*p[i];			
		}


		for(i=0;i<N;i++)
		{
			pt+=p1[i]*p[i];
			rt+=r[i]*r[i];		
		}
		alpha=rt/pt;
		////////////////////////////////////

		/////////////////////////// STEP 4
		for(i=0;i<N;i++)
		{
			x_temp[i]+=alpha*p[i];
			e+=fabs(x_temp[i]-X[i]);
		}
		////////////////////////////////////


		rt=0; pt=0; k=0;		
		for(k=0;k<count;k++)
		{
			i=Ar[k]%N;
			j=Ar[k]/N;
			r_temp[j]-=alpha*At[k]*p[i];				
		}

		for(j=0;j<N;j++)
		{
			r_temp[j]+=r[j];
			rt+=r_temp[j]*r_temp[j];
			pt+=r[j]*r[j];
		}

		beta=rt/pt;  ////////////////////////////////////////////  STEP 7
		/////////////////////////////////////////////////  STEP 8
		for(i=0;i<N;i++)
		{
			p[i]=r_temp[i]+beta*p[i];
			r[i]=r_temp[i];
			X[i]=x_temp[i];
		}
		
	}while(e>es);

	free(x_temp);
	free(r);
	free(r_temp);
	free(p);
	free(p1);

	free(At);
	free(Ar);
}


void BiCG(double *A, double *B, double *X,int N)
{
	int i,j,n=0,NN,k=0;
	double alpha,rt=0,e=0;
	double beta,es=0.00001,sum=0;
	double pt=0;
	int count=0;

	double *AT		=(double *)calloc(sizeof(double),N*N);


	double *x_temp	=(double *)calloc(sizeof(double),N);
	double *r		=(double *)calloc(sizeof(double),N);
	double *r_temp	=(double *)calloc(sizeof(double),N);
	double *p		=(double *)calloc(sizeof(double),N);
	double *p1		=(double *)calloc(sizeof(double),N);

	////////////////////////  hat  /////////////////////////////
	double *x_temp__	=(double *)calloc(sizeof(double),N);
	double *r__			=(double *)calloc(sizeof(double),N);
	double *r_temp__	=(double *)calloc(sizeof(double),N);
	double *p__			=(double *)calloc(sizeof(double),N);
	double *p1__		=(double *)calloc(sizeof(double),N);


	///////////////////   Transpose Matrix   ////////////////
	for(j=0;j<N;j++)
	{
		for(i=0;i<N;i++)
		{
			AT[i+j*N]=A[j+i*N];
		}
	}

	///////////////////////////  length of Sparse matrix  ///////////////////////
	for(i=0;i<N*N;i++)
	{
		if(A[i]!=0.0)
		{
			count++;
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////  Sparse Matrix  and  Interger matrix  //////////////////	
	double *At		=(double *)calloc(sizeof(double),count);
	int    *Ar		=(int    *)calloc(sizeof(int)   ,count);

	double *At__	=(double *)calloc(sizeof(double),count);
	int    *Ar__	=(int    *)calloc(sizeof(int)   ,count);

	////////////////////////////////////////////////////////////////////////////////
	count=0;
	for(i=0;i<N*N;i++)
	{
		if(A[i]!=0.0)
		{
			At[count]=A[i];
			Ar[count]=i;
			count++;
		}
	}
	count=0;
	for(i=0;i<N*N;i++)
	{
		if(AT[i]!=0.0)
		{
			At__[count]=AT[i];
			Ar__[count]=i;
			count++;
		}
	}
	////////////////////////////////////////////////////////////////////////////////




	for(i=0;i<N;i++)  //////////////////  Initial x matrix (x0) 임의로 결정
	{
		X[i]=x_temp[i]=x_temp__[i]=1;
	}



	k=0;
	sum=0;
	for(k=0;k<count;k++)
	{
		i=Ar[k]%N;
		j=Ar[k]/N;
		r[j]+=At[k]*X[i];
	}
	for(j=0;j<N;j++)
	{

		r[j]=B[j]-r[j];  /////////////  STEP 1
		p[j]=r[j];
	}

	k=0;
	sum=0;
	for(k=0;k<count;k++)
	{
		i=Ar__[k]%N;
		j=Ar__[k]/N;
		r__[j]+=At__[k]*X[i];
	}
	for(j=0;j<N;j++)
	{

		r__[j]=B[j]-r__[j];  /////////////  STEP 1
		p__[j]=r__[j];
	}




	do  //////////////////////////  STEP 2
	{
		//////////////////////////  STEP 3
		pt=rt=e=k=0;

		//////////////  Initialize  /////////////////
		for(j=0;j<N;j++)
		{
			p1[j]=r_temp[j]=p1__[j]=r_temp__[j]=0;
		}


		for(k=0;k<count;k++)
		{
			i=Ar[k]%N;
			j=Ar[k]/N;
			p1[j]+=At[k]*p[i];			
		}


		for(i=0;i<N;i++)
		{
			pt+=p__[i]*p1[i];
			rt+=r__[i]*r[i];		
		}
		alpha=rt/pt;
		////////////////////////////////////

		/////////////////////////// STEP 4
		for(i=0;i<N;i++)
		{
			x_temp[i]	+=	alpha*	p[i];
			x_temp__[i]	+=	alpha*	p__[i];

			e+=fabs(x_temp[i]-X[i]);
		}
		////////////////////////////////////


		rt=0; pt=0; k=0;		
		for(k=0;k<count;k++)
		{
			i=Ar[k]%N;
			j=Ar[k]/N;
			r_temp[j]-=alpha*At[k]*p[i];				
		}
		for(k=0;k<count;k++)
		{
			i=Ar__[k]%N;
			j=Ar__[k]/N;
			r_temp__[j]-=alpha*At__[k]*p__[i];				
		}



		for(j=0;j<N;j++)
		{
			r_temp[j]+=r[j];
			r_temp__[j]+=r__[j];
			rt+=r_temp__[j]*r_temp[j];
			pt+=r__[j]*r[j];
		}


		beta=rt/pt;  ////////////////////////////////////////////  STEP 7
		/////////////////////////////////////////////////  STEP 8
		for(i=0;i<N;i++)
		{
			p[i]=r_temp[i]+beta*p[i];
			p__[i]=r_temp__[i]+beta*p__[i];
			r[i]=r_temp[i];
			r__[i]=r_temp__[i];
			X[i]=x_temp[i];
		}
		
	}while(e>es);
}


void TDMA(double *A, double *B, double *X,int n)
{
	int i, j;
	double *P,*Q;
	P=(double *)calloc(sizeof(double),n);
	Q=(double *)calloc(sizeof(double),n);		
	P[0]=-(A[1+n*0]/A[0+n*0]);
	Q[0]=B[0]/A[0+n*0];

	for(i=1;i<n;i++)
	{
		P[i]=-A[i+1+n*i]/(A[i+n*i]+A[(i-1)+n*i]*P[i-1]);
		Q[i]=(B[i]-A[(i-1)+n*i]*Q[i-1])/(A[i+n*i]+A[(i-1)+n*i]*P[i-1]);
	}
	X[n-1]=Q[n-1];
	
	for(i=n-2;i>-1;i--)  // X행렬 구하는 반복문
	{
		X[i]=P[i]*X[i+1]+Q[i];
	}
}

void LU(double *A , double *b , double *x ,int length)     /////////     A*x=b    /////////
{
	int i,j,k,p;
	double sum;

	double *L=(double *)calloc(sizeof(double),length*length);
	double *U=(double *)calloc(sizeof(double),length*length);

	double *c=(double *)calloc(sizeof(double),length);

	for(i=0;i<length;i++)
	{
		U[i+i*length]=1.0;
	}
	for(j=0;j<length;j++)
	{
		L[0+j*length]=A[0+j*length];
	}
	for(i=0;i<length;i++)
	{
		U[i+0*length]=A[i+0*length]/L[0+0*length];
	}
	for(i=1;i<length-1;i++)
	{
		for(j=i;j<length;j++)
		{
			sum=0;
			for(k=0;k<i;k++)
			{
				sum+=L[k+j*length]*U[i+k*length];
			}
			L[i+j*length]=A[i+j*length]-sum;
		}
		for(k=i+1;k<length;k++)
		{
			sum=0;
			for(p=0;p<i;p++)
			{
				sum+=L[p+i*length]*U[k+p*length];
			}
			U[k+i*length]=(A[k+i*length]-sum)/L[i+i*length];
		}
	}
	sum=0;
	for(k=0;k<length;k++)
	{
		sum+=L[k+(length-1)*length]*U[(length-1)+k*length];
	}
	L[(length-1)+(length-1)*length]=A[(length-1)+(length-1)*length]-sum;

	

	c[0]=b[0]/L[0+0*length];
	for(i=1;i<length;i++)
	{
		sum=0;
		for(j=0;j<i;j++)
		{
			sum+=L[j+i*length]*c[j];
		}
		c[i]=(b[i]-sum)/L[i+i*length];
	}


	x[length-1]=c[length-1];
	for(i=length-2;i>-1;i--)
	{
		sum=0;
		for(j=length-1;j>i;j--)
		{
			sum+=U[j+i*length]*x[j];
		}
		x[i]=c[i]-sum;
	}

	free(L);
	free(U);
	free(c);
}
