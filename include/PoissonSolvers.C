

#include "PoissonSolvers.h"
#include <fftw3.h>



using namespace std;
using namespace lux;



void lux::FFTDivFree( BaseGrid<Vector>* divfreeU )
{
   fftw_complex *datax, *datay, *dataz;
   fftw_plan pforwardx, pbackwardx;
   fftw_plan pforwardy, pbackwardy;
   fftw_plan pforwardz, pbackwardz;
   datax = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * divfreeU->NX()*divfreeU->NY()*divfreeU->NZ());
   datay = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * divfreeU->NX()*divfreeU->NY()*divfreeU->NZ());
   dataz = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * divfreeU->NX()*divfreeU->NY()*divfreeU->NZ());
   pforwardx = fftw_plan_dft_3d( divfreeU->NX(), divfreeU->NY(), divfreeU->NZ(), datax, datax, FFTW_FORWARD, FFTW_ESTIMATE);
   pbackwardx = fftw_plan_dft_3d( divfreeU->NX(), divfreeU->NY(), divfreeU->NZ(), datax, datax, FFTW_BACKWARD, FFTW_ESTIMATE);
   pforwardy = fftw_plan_dft_3d( divfreeU->NX(), divfreeU->NY(), divfreeU->NZ(), datay, datay, FFTW_FORWARD, FFTW_ESTIMATE);
   pbackwardy = fftw_plan_dft_3d( divfreeU->NX(), divfreeU->NY(), divfreeU->NZ(), datay, datay, FFTW_BACKWARD, FFTW_ESTIMATE);
   pforwardz = fftw_plan_dft_3d( divfreeU->NX(), divfreeU->NY(), divfreeU->NZ(), dataz, dataz, FFTW_FORWARD, FFTW_ESTIMATE);
   pbackwardz = fftw_plan_dft_3d( divfreeU->NX(), divfreeU->NY(), divfreeU->NZ(), dataz, dataz, FFTW_BACKWARD, FFTW_ESTIMATE);

   long index = 0;
   for( int k=0;k<divfreeU->NZ();k++ )
   {
      for( int j=0;j<divfreeU->NY();j++ )
      {
         for( int i=0;i<divfreeU->NX();i++ )
	 {
            datax[index][0] = divfreeU->get(i,j,k)[0];
            datax[index][1] = 0;
            datay[index][0] = divfreeU->get(i,j,k)[1];
            datay[index][1] = 0;
            dataz[index][0] = divfreeU->get(i,j,k)[2];
            dataz[index][1] = 0;
            ++index;
	 }
      }
   }


   fftw_execute(pforwardx);
   fftw_execute(pforwardy);
   fftw_execute(pforwardz);


   float dKx = 2.0 * M_PI  / divfreeU->NX();
   float dKy = 2.0 * M_PI  / divfreeU->NY();
   float dKz = 2.0 * M_PI  / divfreeU->NZ();
   float nyqK = 2.0 * M_PI;
   float scaler = 1.0/(divfreeU->NX() * divfreeU->NY() * divfreeU->NZ() );
   index = 0;
   for( int k=0;k<divfreeU->NZ();k++ )
   {
      float kz = k * dKz;
      if( k > divfreeU->NZ()/2 ){ kz -= nyqK; }
      for( int j=0;j<divfreeU->NY();j++ )
      {
         float ky = j * dKy;
         if( j > divfreeU->NY()/2 ){ ky -= nyqK; }
         for( int i=0;i<divfreeU->NX();i++ )
         {
	    float kx = i * dKx;
            if( i > divfreeU->NX()/2 ){ kx -= nyqK; }
	    float ksq = kx*kx + ky*ky + kz*kz;

            if( ksq == 0.0 )
	    {
	       datax[index][0] = datax[index][1] = 0.0;
	       datay[index][0] = datay[index][1] = 0.0;
	       dataz[index][0] = dataz[index][1] = 0.0;
	    }
            else
	    {
            float vdotkr = kx*datax[index][0] + ky*datay[index][0] + kz*dataz[index][0];
            float vdotki = kx*datax[index][1] + ky*datay[index][1] + kz*dataz[index][1];

            datax[index][0] -= vdotkr * kx / ksq;
            datay[index][0] -= vdotkr * ky / ksq;
            dataz[index][0] -= vdotkr * kz / ksq;

            datax[index][1] -= vdotki * kx / ksq;
            datay[index][1] -= vdotki * ky / ksq;
            dataz[index][1] -= vdotki * kz / ksq;

	    datax[index][0] *= scaler;
	    datay[index][0] *= scaler;
	    dataz[index][0] *= scaler;

	    datax[index][1] *= scaler;
	    datay[index][1] *= scaler;
	    dataz[index][1] *= scaler;
            }
	    ++index;
         }
      }
   }


   fftw_execute(pbackwardx);
   fftw_execute(pbackwardy);
   fftw_execute(pbackwardz);


   index = 0;
   for( int k=0;k<divfreeU->NZ();k++ )
   {
      for( int j=0;j<divfreeU->NY();j++ )
      {
         for( int i=0;i<divfreeU->NX();i++ )
	 {
	    divfreeU->set(i,j,k,Vector( datax[index][0], datay[index][0], dataz[index][0] ));
	    ++index;
         }
      }
   }


   fftw_destroy_plan(pforwardx);
   fftw_destroy_plan(pbackwardx);
   fftw_free(datax);

   fftw_destroy_plan(pforwardy);
   fftw_destroy_plan(pbackwardy);
   fftw_free(datay);
   
   fftw_destroy_plan(pforwardz);
   fftw_destroy_plan(pbackwardz);
   fftw_free(dataz);
}
