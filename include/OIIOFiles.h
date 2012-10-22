

#ifndef __OIIOFILES_H__
#define __OIIOFILES_H__

#include "Image.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

namespace lux{


void writeOIIOImage( const char* fname, Image& img, float brightness = 1.0, float gamma = 1.0 );
void writeOIIOImage( const char* fname, Image& img, const map<string,string>& labels, float brightness = 1.0, float gamma = 1.0 );
void writeOIIOImage( const char* fname, Image& img, const vector<string>& keys, const vector<string>& tags,  float brightness = 1.0, float gamma = 1.0 );
void readOIIOImage( const char* fname, Image& img );
void readOIIOImage( const char* fname, Image& img, map<string,string>& labels );
void readOIIOImage( const char* fname, Image& img, vector<string>& keys, vector<string>& tags );
void printMetadata( const map<string,string>& meta );


}

#endif


