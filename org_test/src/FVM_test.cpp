#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix_solver.h"

#define Iter 41

int main(void)
{
   int a,i,j,k,I,J,K,N,m,n,iter;
   int ID1, ID2, ID3, ID4, index;
   int nPoints, nFaces, ninternalFaces, nCells;
   int *owner, *boundary, *Cell, *Face, *neighbour;

   double x1,y1,z1,x2,y2,z2,x3,y3,z3,Sx,Sy,Sz,Area,Vol,Temp;
   double dt,den,C,gamma,Aj;
   double B_C_temperature;
   double *Point, *Volume, *Surface, *Volgrad, *Cellcen;
   double *A, *X, *B;

   /////////////////////////////////////   DATA reading & writing   //////////////////////////////////////////////////////////////
   FILE *fp1;   FILE *fp2;   FILE *fp3;   FILE *fp4;   FILE *fp5;                                                   //
   FILE *fp6;   FILE *fp7;   FILE *fp8;   FILE *fp9;   FILE *fp10;                                                   //
   char fname1[512];   char fname2[512];   char fname3[512];   char fname4[512];   char fname5[512];                     //
   char fname6[512];   char fname7[512];   char fname8[512];   char fname9[512];   char fname10[512];                     //
                                                                                                //
   sprintf(fname1,"polyMesh/lockMeshDict");                                                               //
   sprintf(fname2,"polyMesh/boundary");                                                                  //
   sprintf(fname3,"polyMesh/faces");                                                                     //
   sprintf(fname4,"polyMesh/neighbour");                                                                  //
   sprintf(fname5,"polyMesh/owner");                                                                     //
   sprintf(fname6,"polyMesh/points");                                                                     //
   sprintf(fname7,"1/point data.dat");                                                                  //
   sprintf(fname8,"1/cell volume.dat");                                                                  //
   sprintf(fname9,"1/temperature.dat");                                                                  //
   sprintf(fname10,"1/A matrix.dat");                                                                     //
                                                                                                //
   fp1=fopen(fname1,"r");   fp2=fopen(fname2,"r");   fp3=fopen(fname3,"r");   fp4=fopen(fname4,"r");   fp5=fopen(fname5,"r");      //
   fp6=fopen(fname6,"r");   fp7=fopen(fname7,"w");   fp8=fopen(fname8,"w");   fp9=fopen(fname9,"w");   fp10=fopen(fname10,"w");   //
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



   fscanf(fp6,"%d\n",&nPoints);   //   nPoints�� Point���Ͽ��� �д´�
   fscanf(fp3,"%d\n",&nFaces);      //   nFaces�� Face���Ͽ��� �д´�
//   fscanf(fp3,"%d\n",ninternalFaces);
//   fscanf(fp3,"%d\n",nCells);

   nCells=640;               //   nCell�� �˰��ִ� ���� �����Ѵ�
   ninternalFaces=1648;      //   ninternalFaces�� �˰��ִ� ���� �����Ѵ� 

   dt=0.001;
   den=C=gamma=Aj=1.0;
   B_C_temperature=1.0;

   A=(double*)calloc(sizeof(double),nCells*nCells);   //   AX=B
   X=(double*)malloc(sizeof(double)*nCells);         //   AX=B
   B=(double*)calloc(sizeof(double),nCells);         //   AX=B

   neighbour=(int*)calloc(sizeof(int),ninternalFaces);   //   neighbour file read
   owner=(int*)calloc(sizeof(int),nFaces);            //   owner file read
   boundary=(int*)calloc(sizeof(int),nFaces);         //   boundary count : 0= internal, 1=flux, 2=temperatura,.....
   Face=(int*)calloc(sizeof(int),nFaces*4);         //   Face file read
   Cell=(int*)calloc(sizeof(int),nCells*6);         //   Cell(face1, face2, face3, face4, face5, face6)   
   
   Point=(double*)calloc(sizeof(double),nPoints*3);   //   Point file read
   Volume=(double*)calloc(sizeof(double),nCells);      //   calculated volume 
   Surface=(double*)calloc(sizeof(double),nFaces*3);   //   calculated surface vector
   Volgrad=(double*)calloc(sizeof(double),nCells*3);   //   volume gradient
   Cellcen=(double*)calloc(sizeof(double),nCells*3);   //   volume center position

   printf("      nPoints=%d\n",nPoints);
   printf("      nFaces=%d\n",nFaces);
   printf("      nCells=%d\n",nCells);
   printf("      ninternalFaces=%d\n",ninternalFaces);   
   
   for(i=0;i<ninternalFaces;i++)   //   neighbor read
   {
      fscanf(fp4,"%d\n",&neighbour[i]);
   }

   for(i=0;i<nFaces;i++)         //   owner read
   {
      fscanf(fp5,"%d\n",&owner[i]);
   }

   for(i=0;i<nFaces;i++)         //   Face read
   {
      fscanf(fp3,"%d %d %d %d\n",&Face[i*4+0],&Face[i*4+1],&Face[i*4+2],&Face[i*4+3]);
   }

   for(i=0;i<nPoints;i++)         //   point read
   {
      fscanf(fp6,"%lf %lf %lf\n",&Point[i*3+0],&Point[i*3+1],&Point[i*3+2]);
      fprintf(fp7,"%f %f %f\n",Point[i*3+0],Point[i*3+1],Point[i*3+2]);      
   }
   
//   for(size_t cnt=0;cnt<nFaces;cnt++)
//   {
//	   printf("%d %d %d %d \n", Face[cnt*4+0],Face[cnt*4+1],Face[cnt*4+2],Face[cnt*4+3]);
//   }

//	for(size_t cnt=0;cnt<ninternalFaces;cnt++)
//	{
//		printf("%d\n", neighbour[cnt]);
//	}

//	for(size_t cnt=0;cnt<nFaces;cnt++)
//	{
//		printf("%d\n", owner[cnt]);
//	}

//	for(size_t cnt=0;cnt<nPoints;cnt++)
//	{
//		printf("%lf %lf %lf\n", Point[cnt*3+0],Point[cnt*3+1],Point[cnt*3+2]);
//	}

   for(i=0;i<nCells;i++)   X[i]=0.0;

   for(i=0;i<nCells;i++)   // Cell �迭 ��
   {
      for(a=0;a<6;a++)
      {
         Cell[i*6+a]=-1;
      }
   }

   for(i=0;i<ninternalFaces;i++)   // Cell �迭 ��
   {
      if (Cell[neighbour[i]*6+0]==-1)
      {
         Cell[neighbour[i]*6+0]=i;
      }
      else if (Cell[neighbour[i]*6+1]==-1)
      {
         Cell[neighbour[i]*6+1]=i;
      }
      else if (Cell[neighbour[i]*6+2]==-1)
      {
         Cell[neighbour[i]*6+2]=i;
      }
      else if (Cell[neighbour[i]*6+3]==-1)
      {
         Cell[neighbour[i]*6+3]=i;
      }
      else if (Cell[neighbour[i]*6+4]==-1)
      {
         Cell[neighbour[i]*6+4]=i;
      }
      else if (Cell[neighbour[i]*6+5]==-1)
      {
         Cell[neighbour[i]*6+5]=i;
      }   
   }

   for(i=0;i<nFaces;i++)   // Cell �迭 ��
   {
      if (Cell[owner[i]*6+0]==-1)
      {
         Cell[owner[i]*6+0]=i;
      }
      else if (Cell[owner[i]*6+1]==-1)
      {
         Cell[owner[i]*6+1]=i;
      }
      else if (Cell[owner[i]*6+2]==-1)
      {
         Cell[owner[i]*6+2]=i;
      }
      else if (Cell[owner[i]*6+3]==-1)
      {
         Cell[owner[i]*6+3]=i;
      }
      else if (Cell[owner[i]*6+4]==-1)
      {
         Cell[owner[i]*6+4]=i;
      }
      else if (Cell[owner[i]*6+5]==-1)
      {
         Cell[owner[i]*6+5]=i;
      }   
   }

//   for(size_t cnt=0;cnt<nCells;++cnt)
//   {
//	   for(size_t cnt2=0;cnt2<6;++cnt2)
//	   {
//		   printf("%d ", Cell[cnt * 6 + cnt2]);
//	   }
//	   printf("\n");
//   }

   for(i=ninternalFaces;i<nFaces;i++)   // boundary ��
   {
      boundary[i]=2;
   }

//	for(size_t cnt=0;cnt<nFaces;++cnt)
//	{
//	   printf("%d\n", boundary[cnt]);
//	}

   ///////////////   Surface vector   //////////////
   for(i=0;i<nFaces;i++)               //
   {                     //
      ID1=Face[i*4+0];   ID3=Face[i*4+2];   //
                        //
      x1=Point[ID3*3+0]-Point[ID1*3+0];      //
      y1=Point[ID3*3+1]-Point[ID1*3+1];      //
      z1=Point[ID3*3+2]-Point[ID1*3+2];      //
                        //
      ID2=Face[i*4+1];   ID4=Face[i*4+3];   //
                        //
      x2=Point[ID4*3+0]-Point[ID2*3+0];      //
      y2=Point[ID4*3+1]-Point[ID2*3+1];      //
      z2=Point[ID4*3+2]-Point[ID2*3+2];      //
                                    //
      Sx=0.5*(y1*z2 - y2*z1);               //
      Sy=0.5*(z1*x2 - z2*x1);               //
      Sz=0.5*(x1*y2 - x2*y1);               //
                                    //
      Surface[i*3+0]=Sx;                  //
      Surface[i*3+1]=Sy;                  //
      Surface[i*3+2]=Sz;                  //
   }                                 //
   //////////////////////////////////////////////

	//for(size_t cnt=0;cnt<nFaces;++cnt)
	//{
	//	printf("%lf %lf %lf\n", Surface[cnt*3+0], Surface[cnt*3+1], Surface[cnt*3+2]);
	//}

   /////////////////////////////////   hexahedral Volume calculation   ///////////////////////////////////
   for(i=0;i<nCells;i++)
   {
      Vol=0.0;

      m=Cell[i*6+0];
      ID4=Face[m*4+0];
      for(j=1;j<6;j++)
      {
         n=Cell[i*6+j];
         index=0;
         for(k=0;k<4;k++)
         {
        	 if(ID4==Face[n*4+k])
        	 {
        		 index=1;
        	 }
         }

         if(index==0)
         {
            ID1=Face[n*4+0];   ID2=Face[n*4+1];   ID3=Face[n*4+2];
            /////////////////   Tetrahedral Cell volume   /////////////////////////////////
            x1=Point[ID1*3+0]-Point[ID4*3+0];
            y1=Point[ID1*3+1]-Point[ID4*3+1];
            z1=Point[ID1*3+2]-Point[ID4*3+2];      
   
            x2=Point[ID2*3+0]-Point[ID4*3+0];
            y2=Point[ID2*3+1]-Point[ID4*3+1];
            z2=Point[ID2*3+2]-Point[ID4*3+2];      
   
            x3=Point[ID3*3+0]-Point[ID4*3+0];
            y3=Point[ID3*3+1]-Point[ID4*3+1];
            z3=Point[ID3*3+2]-Point[ID4*3+2];

            Vol+=(x1*(y2*z3 - y3*z2) + y1*(z2*x3 - z3*x2) + z1*(x2*y3 - x3*y2))/6.0;
            ////////////////////////////////////////////////////////////////////////////

            ID1=Face[n*4+2];   ID2=Face[n*4+3];   ID3=Face[n*4+0];
            /////////////////   Tetrahedral Cell volume   /////////////////////////////////
            x1=Point[ID1*3+0]-Point[ID4*3+0];
            y1=Point[ID1*3+1]-Point[ID4*3+1];
            z1=Point[ID1*3+2]-Point[ID4*3+2];      
   
            x2=Point[ID2*3+0]-Point[ID4*3+0];
            y2=Point[ID2*3+1]-Point[ID4*3+1];
            z2=Point[ID2*3+2]-Point[ID4*3+2];      
   
            x3=Point[ID3*3+0]-Point[ID4*3+0];
            y3=Point[ID3*3+1]-Point[ID4*3+1];
            z3=Point[ID3*3+2]-Point[ID4*3+2];

            Vol+=(x1*(y2*z3 - y3*z2) + y1*(z2*x3 - z3*x2) + z1*(x2*y3 - x3*y2))/6.0;
            //////////////////////////////////////////////////////////////////////////////
         }
      }
      Volume[i]=Vol;
      //printf("%.16f\n",Vol);
   }

	//for(size_t cnt=0;cnt<nCells;++cnt)
	//{
	//	printf("%lf\n", Volume[cnt]);
	//}

   //////////////////////////////////////////////////////////////////////////////////////
   printf("Volume=%f\n\n",Surface[4]);
   ////////////////////////////   Volume center   ////////////////////////////////////
   for(i=0;i<nCells;i++)
   {
      for(j=0;j<6;j++)
      {
         n=Cell[i*6+j];   //   n:face number
         for(k=0;k<4;k++)
         {
            m=Face[n*4+k];   //   m:point number

            Cellcen[i*3+0]+=Point[m*3+0];
            Cellcen[i*3+1]+=Point[m*3+1];
            Cellcen[i*3+2]+=Point[m*3+2];
         }
      }
      Cellcen[i*3+0]/=24.0;
      Cellcen[i*3+1]/=24.0;
      Cellcen[i*3+2]/=24.0;
   }
   ////////////////////////////////////////////////////////////////////////////////////
   
	//for(size_t cnt=0;cnt<nCells;cnt++)
	//{
	//	printf("%lf %lf %lf\n", Cellcen[cnt*3+0],Cellcen[cnt*3+1],Cellcen[cnt*3+2]);
	//}


   for(iter=0;iter<Iter;iter++)
   {
      ////////////////////////////   Volume gradient   ////////////////////////////////////
      for(i=0;i<nCells;i++)
      {
         Volgrad[i*3+0]=0.0;
         Volgrad[i*3+1]=0.0;
         Volgrad[i*3+2]=0.0;

         for(j=0;j<6;j++)
         {
            n=Cell[i*6+j];   //   n:face number

            if(i==owner[n])      //   surface vector direction
            {
               Sx= Surface[n*3+0];   
               Sy= Surface[n*3+1];
               Sz= Surface[n*3+2];
            }
            else            //   surface vector direction
            {
               Sx= -Surface[n*3+0];
               Sy= -Surface[n*3+1];
               Sz= -Surface[n*3+2];
            }

            Area=sqrt(Sx*Sx + Sy*Sy + Sz*Sz);   //   |sj| ���

            if(n<ninternalFaces)   //   n is internal faces
            {
               I=owner[n];      //   I:owner cell number
               J=neighbour[n];   //   J:neighbour cell number                     
   
               Temp=0.5*(X[I] + X[J]);   // Temp:face temperature
               
               Volgrad[i*3+0]+=Temp*Sx;
               Volgrad[i*3+1]+=Temp*Sy;
               Volgrad[i*3+2]+=Temp*Sz;
            }
            else               //   n is boundary
            {
               if(boundary[n]==1)   //   Flux boundary
               {
   //               Temp=(h0/h0+D)*B[i] + (C-S0)/(h0+D);   // Temp:face temperature
   //
   //               Volgrad[i*3+0]+=Temp*Sx;
   //               Volgrad[i*3+1]+=Temp*Sy;
   //               Volgrad[i*3+2]+=Temp*Sz;
               }
               if(boundary[n]==2)   //   temperature boundary
               {
                  Temp=B_C_temperature;   // Temp:face temperature
   
                  Volgrad[i*3+0]+=Temp*Sx;
                  Volgrad[i*3+1]+=Temp*Sy;
                  Volgrad[i*3+2]+=Temp*Sz;
               }
            }
         }

         Volgrad[i*3+0]/=Volume[i];
         Volgrad[i*3+1]/=Volume[i];
         Volgrad[i*3+2]/=Volume[i];
      }
      /////////////////////////////////////////////////////////////////////

//      for(n=0;n<nCells;n++)   X[n]=0.0;   //   matrix initialization
      //////////////////////////////   MATRIX   //////////////////////////////////////
      for(m=0;m<nCells;m++)
      {
         for(n=0;n<nCells;n++)   A[m*nCells+n]=0.0;   //   matrix initialization
         //X[m]=B[m]=0.0;
         B[m]=0.0;

         A[m*nCells+m]+=den*Volume[m]/dt;
         B[m]+=X[m]*den*Volume[m]/dt;

         //printf("%d-%f %f %f %f %f\n",m,X[m],den,Volume[m],dt, B[m]);
         for(j=0;j<6;j++)   
         {
            i=Cell[m*6+j];   //   i:face number

            if(m==owner[i])      //   surface vector direction
            {
               Sx= Surface[i*3+0];   
               Sy= Surface[i*3+1];
               Sz= Surface[i*3+2];
            }
            else            //   surface vector direction
            {
               Sx= -Surface[i*3+0];
               Sy= -Surface[i*3+1];
               Sz= -Surface[i*3+2];
            }         
   
            Area=sqrt(Sx*Sx + Sy*Sy + Sz*Sz);   //   |sj| ���

            if(i<ninternalFaces)
            { 
               if(m==owner[i])   J=neighbour[i];   //   J: j���� cell ��ȣ
               else         J=owner[i];      //   J: j���� cell ��ȣ

               x1=Cellcen[J*3+0]-Cellcen[m*3+0];   //   dsj ��� 
               y1=Cellcen[J*3+1]-Cellcen[m*3+1];   //   dsj ���
               z1=Cellcen[J*3+2]-Cellcen[m*3+2];   //   dsj ���

               x2=Sx/Area;      //   unit normal vector ���
               y2=Sy/Area;      //   unit normal vector ���
               z2=Sz/Area;      //   unit normal vector ���

               x3=Volgrad[m*3+0];   //   volume gradient
               y3=Volgrad[m*3+1];   //   volume gradient
               z3=Volgrad[m*3+2];   //   volume gradient

               Aj=Area/(x1*x2 + y1*y2 + z1*z2);

               A[m*nCells+m]+=gamma*Aj;   //   A0 ���
               A[m*nCells+J]-=gamma*Aj;   //   Aj ���       
               B[m]+=gamma*((x3*Sx + y3*Sy + z3*Sz) - (x3*x1 + y3*y1 + z3*z1)*Aj);   //   B ���
            }
            else
            { 
               if(boundary[i]==1)   // flux boundary
               {
                  /* add code for boundary 1 */
               }
               else if(boundary[i]==2)   // temerature boundary
               {
                  x2=Sx/Area;      //   unit normal vector ���
                  y2=Sy/Area;      //   unit normal vector ���
                  z2=Sz/Area;      //   unit normal vector ���

                  x1=0.025*x2/2.;   //   dsj ���
                  y1=0.025*y2/2.;   //   dsj ���               ////////////////////////////////// �����κ� (������ ���� 0.025/2 �� �������Ϳ� ����)
                  z1=0.025*z2/2.;   //   dsj ���

                  x3=Volgrad[m*3+0];   //   volume gradient
                  y3=Volgrad[m*3+1];   //   volume gradient
                  z3=Volgrad[m*3+2];   //   volume gradient

                  Aj=Area/(x1*x2 + y1*y2 + z1*z2);

                  A[m*nCells+m]+=gamma*Aj;   //   A0 ���
                  A[m*nCells+J]+=0.0;         //   Aj ���       
                  B[m]+= gamma*Aj*B_C_temperature + gamma*((x3*Sx + y3*Sy + z3*Sz) - (x3*x1 + y3*y1 + z3*z1)*Aj);   //   B ��� //////////////////////////////////////// �����κ� h0*Tb �߰�
               }
            }
         }
      }
      ////////////////////////////////////////////////////////////////////////   

      CG(A,B,X,nCells);

      if(iter%10==0)   // Data ���
      {
         fprintf(fp9,"zone\n");
         for(i=0;i<nCells;i++)
         {
            fprintf(fp9,"%f   %f   %f   %f\n",Cellcen[i*3+0],Cellcen[i*3+1],Cellcen[i*3+2],X[i]);
         }
      }

      printf("iter=%d/%d\n",iter,Iter);
   }




   for(m=0;m<nCells;m++)
   {
      for(n=0;n<nCells;n++)
      {
         if(A[m*nCells+n]==A[n*nCells+m]){}
         else
         {
            printf("A[%d*nCells+%d]=%f A[%d*nCells+%d]=%f\n",m,n,A[m*nCells+n],n,m,A[n*nCells+m]);
         }
         fprintf(fp10,"%f   ",A[m*nCells+n]);
      }
      fprintf(fp10,"\n");
   }

   

   for(i=0;i<nCells;i++)
   {
      I=0;
      for(j=0;j<6;j++)
      {
         J=Cell[i*6+j];
         I+=boundary[J];
      }

      fprintf(fp8,"%d   %d   %f   %f   %f\n",i,I,Volgrad[i*3+0],Volgrad[i*3+1],Volgrad[i*3+2]);
      
      for(j=0;j<6;j++)
      {
         n=Cell[i*6+j];

         if(i==owner[n])      //   surface vector direction
         {
            Sx= Surface[n*3+0];
            Sy= Surface[n*3+1];
            Sz= Surface[n*3+2];
         }
         else            //   surface vector direction
         {
            Sx= -Surface[n*3+0];
            Sy= -Surface[n*3+1];
            Sz= -Surface[n*3+2];
         }
         
         if(n<ninternalFaces)
         {
            I=owner[n];      //   I:owner cell number
            J=neighbour[n];   //   J:neighbour cell number

            Temp=0.5*(X[I] + X[J]);   // Temp:face temperature
         }
         else
         {
            Temp=B_C_temperature;
         }

         fprintf(fp8,"%f   %f   %f   %d\n",Sx,Sy,Sz,boundary[n]);
      }
   }


return 0;
}
