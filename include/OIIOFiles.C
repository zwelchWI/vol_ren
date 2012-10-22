
#include "OIIOFiles.h"

#include <iostream>
#include <cmath>
#include <OpenImageIO/imageio.h> 
#include <list>
#include <ctime>
#include <sstream>

using namespace std;
using namespace lux;
OIIO_NAMESPACE_USING


float imagePlaneValue( float v, float dG, float dB )
{
   if( v == 0.0 ){ return 0.0; }
   return pow(v, dG) * dB;
}



void lux::writeOIIOImage( const char* fname, Image& img, float displayBrightness, float displayGamma  )
{
   float* imagedata = new float[ img.Width()* img.Height() * 3 ];

   // fill image with the contents of img

   long index = 0;
   for( int j=0;j<img.Height();j++ )
   {
      for( int i=0;i<img.Width();i++ )
      {
         vector<float> pix = img.pixel(i,img.Height() - j - 1);
	 for( size_t c=0;c<3;c++ )
         {
            pix[c] = imagePlaneValue( pix[c], displayGamma, displayBrightness );
	    imagedata[index++] = pix[c];
         }
      }
   }

   ImageOutput *out = ImageOutput::create (fname); 
   if( !out )
   {
      cout << "Not able to write an image to file " << fname << endl;
   }
   else
   {
      ImageSpec spec (img.Width(), img.Height(), 3, TypeDesc::FLOAT); 
      spec.attribute("user", "imageTools");
      spec.attribute("writer", "OIIOFiles" );
      out->open (fname, spec);
      out->write_image (TypeDesc::FLOAT, imagedata); 
      out->close (); 
	   cout <<endl << endl<< "File " << fname << " written to file"<< endl;
      delete out;
   }
   delete[] imagedata;
}


void lux::writeOIIOImage( const char* fname, Image& img, const vector<string>& keys, const vector<string>& tags,  float displayBrightness, float displayGamma )
{
   map<string,string> labels;
   size_t nblabels = keys.size();
   nblabels = ( nblabels < tags.size() )? nblabels : tags.size();
   for( size_t i=0;i<nblabels;i++ )
   {
      labels[ keys[i] ] = tags[i];
   }
   writeOIIOImage( fname, img, labels, displayBrightness, displayGamma );

}


void lux::writeOIIOImage( const char* fname, Image& img, const map<string,string>& labels, float displayBrightness, float displayGamma )
{
   float* imagedata = new float[ img.Width()* img.Height() * 3 ];

   // fill image with the contents of img

   long index = 0;
   for( int j=0;j<img.Height();j++ )
   {
      for( int i=0;i<img.Width();i++ )
      {
         vector<float> pix = img.pixel(i,img.Height() - j - 1);
	 for( size_t c=0;c<3;c++ )
         {
            pix[c] = imagePlaneValue( pix[c], displayGamma, displayBrightness );
	    imagedata[index++] = pix[c];
         }
      }
   }

   ImageOutput *out = ImageOutput::create (fname); 
   if( !out )
   {
      cout << "Not able to write an image to file " << fname << endl;
   }
   else
   {
      ImageSpec spec (img.Width(), img.Height(), 3, TypeDesc::FLOAT); 
      spec.attribute("user", "imageTools");
      spec.attribute("writer", "OIIOFiles" );
      if( labels.size() > 0 )
      {
         map<string,string>::const_iterator lab = labels.begin();
	 while( lab != labels.end() )
	 {
	    const string& name = lab->first;
	    const string& value = lab->second;
	    spec.attribute( name, value );
	    lab++;
	 }
      }
      out->open (fname, spec);
      out->write_image (TypeDesc::FLOAT, imagedata); 
      out->close (); 
	cout <<endl<<endl<< "File " << fname << " written to file"<< endl;
      delete out;
   }
   delete[] imagedata;
}




void lux::readOIIOImage( const char* fname, Image& img  )
{
   int xres, yres, channels;
   ImageInput *in = ImageInput::create (fname);
   if (! in) {return;}
   ImageSpec spec;
   in->open (fname, spec);
   xres = spec.width;
   yres = spec.height;
   channels = spec.nchannels;
   float* pixels = new float[xres*yres*channels];
   in->read_image (TypeDesc::FLOAT, pixels);

   img.reset( xres, yres, channels );
   long index = 0;
   for( int j=0;j<yres;j++)
   {
      for( int i=0;i<xres;i++ )
      {
         for( int c=0;c<channels;c++ )
         {
	    img.value(i,img.Height() - j - 1,c) = pixels[index++];
         }
      }
   }
   in->close ();
   delete in;
   delete[] pixels;
}

void lux::readOIIOImage( const char* fname, Image& img, map<string,string>& labels )
{
   int xres, yres, channels;
   ImageInput *in = ImageInput::create (fname);
   if (! in) {return;}
   ImageSpec spec;
   in->open (fname, spec);
   xres = spec.width;
   yres = spec.height;
   channels = spec.nchannels;
   float* pixels = new float[xres*yres*channels];
   in->read_image (TypeDesc::FLOAT, pixels);

   img.reset( xres, yres, channels );
   long index = 0;
   for( int j=0;j<yres;j++)
   {
      for( int i=0;i<xres;i++ )
      {
         for( int c=0;c<channels;c++ )
         {
	    img.value(i,img.Height() - j - 1,c) = pixels[index++];
         }
      }
   }

   for( size_t i=0;i<spec.extra_attribs.size();i++)
   {
      const ParamValue& p = spec.extra_attribs[i];
      string name = p.name().c_str();
      string value = spec.metadata_val ( p, true);
      labels[name] = value;
   }

   in->close ();
   delete in;
   delete[] pixels;
}



void lux::readOIIOImage( const char* fname, Image& img, vector<string>& keys, vector<string>& tags )
{
   map<string,string> labels;
   readOIIOImage( fname, img, labels );
   map<string,string>::iterator p = labels.begin();
   keys.clear();
   tags.clear();
   while( p != labels.end() )
   {
      keys.push_back( p->first );
      tags.push_back( p->second );
      p++;
   }
}




void lux::printMetadata( const map<string,string>& meta )
{
   if( meta.empty() ){ return; }
   cout << "\n\nMetadata Labels\n==============================\n";
   map<string,string>::const_iterator p = meta.begin();
   size_t maxlength = 0;
   while( p != meta.end() )
   {
      maxlength = ( maxlength < p->first.size() ) ? p->first.size() : maxlength;
      p++;
   }
   maxlength += 1;
   p = meta.begin();
   while( p != meta.end() )
   {
      size_t extraspace = maxlength - p->first.size();
      cout << p->first;
      for( size_t i=0;i<extraspace;i++ ){ cout << " "; }
      cout << "---------> " << p->second << endl;
      p++;
   }
   cout << "\n==============================\n\n";

}
