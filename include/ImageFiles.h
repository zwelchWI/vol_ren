

#ifndef ___IMAGEFILES_H___
#define ___IMAGEFILES_H___

#include "Image.h"
#include "CmdLineFind.h"

namespace lux{


void writePPMImage(CmdLineFind& clf, Image& img);
void writeMagickImage(CmdLineFind& clf, Image& img);






}

#endif
