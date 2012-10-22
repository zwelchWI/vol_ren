#CPSC 819


INCDIR = ./include

MAGICKLIBS =  `Magick++-config  --cppflags --cxxflags --ldflags --libs` 

INCLUDES = #-I ./ `Magick++-config  --cppflags --cxxflags`

OIIO = -I ${IMAGEIO_INCLUDE_PATH}
OIIOOBJECTS =   ${IMAGEIO_LIBRARY_PATH}/libOpenImageIO.*



CXX = g++ -g -O2 -D_THREAD_SAFE -pthread -I ${INCDIR}


HEADERS = ${INCDIR}/Camera.h ${INCDIR}/ColorFields.h ${INCDIR}/ImplicitSurfaces.h ${INCDIR}/OIIOFiles.h	${INCDIR}/Volume.h ${INCDIR}/CmdLineFind.h	${INCDIR}/Image.h	${INCDIR}/LinearAlgebra.h	${INCDIR}/ProgressMeter.h ${INCDIR}/Color.h	 ${INCDIR}/ImageFiles.h ${INCDIR}/Matrix.h	${INCDIR}/Vector.h ${INCDIR}/Density.h ${INCDIR}/BaseGrid.h ${INCDIR}/GridVolume.h ${INCDIR}/SDF.h ${INCDIR}/FullGrid.h ${INCDIR}/CGridVolume.h ${INCDIR}/SparseGrid.h ${INCDIR}/PerlinNoise.h ${INCDIR}/Noise.h ${INCDIR}/UniformPRN.h ${INCDIR}/DSM.h

OBJ = ./obj/Camera.o ./obj/Image.o ./obj/LinearAlgebra.o  ./obj/Matrix.o ./obj/PerlinNoise.o ./obj/PoissonSolvers.o

render: ./obj/render.o ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ ./obj/render.o ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}

./obj/render.o : render.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<
./obj/Camera.o : ${INCDIR}/Camera.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<
./obj/Image.o : ${INCDIR}/Image.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<
./obj/LinearAlgebra.o : ${INCDIR}/LinearAlgebra.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<
./obj/MagickFiles.o : ${INCDIR}/MagickFiles.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<
./obj/Matrix.o: ${INCDIR}/Matrix.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<
./obj/OIIOFiles.o : ${INCDIR}/OIIOFiles.C ${HEADERS} Makefile
	${CXX} ${INCLUES} ${OIIO} -o $@ -c $<
./obj/PerlinNoise.o: ${INCDIR}/PerlinNoise.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<
./obj/PoissonSolvers.o: ${INCDIR}/PoissonSolvers.C ${HEADERS} Makefile
	${CXX} ${INCLUDES} -o $@ -c $<

./obj/.C.o: $(HEADERS) Makefile
	${CXX} ${INCLUDES} -o $@ -c $<

all: render

renderWisp:  renderWisp.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl  -o $@ renderWisp.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}

renderPyro:  renderPyro.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl  -o $@ renderPyro.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}

renderStamp: renderStamp.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl  -o $@ renderStamp.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderFsim: renderFsim.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderFsim.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderRobot: renderRobot.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderRobot.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderFlame: renderFlame.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderFlame.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderSpin: renderFinalSpin.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderFinalSpin.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderEyes: renderEyes.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderEyes.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderShoot: renderShoot.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderShoot.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderLaser: renderLaser.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderLaser.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}
renderBoom: renderBoom.C ./obj/OIIOFiles.o  $(HEADERS) $(OBJ) 
	${CXX}  ${OIIO} -ldl -lfftw3  -o $@ renderBoom.C ./obj/OIIOFiles.o  ${OBJ} ${OIIOOBJECTS}


magick:  $(HEADERS) $(OBJ) 
		${CXX} ${MAGICKLIBS} -o mRender render.C -DMAGICK include/MagickFiles.C ${OBJ} ${INCLUDES}

backup:
	rm -f Render.tar.gz
	tar -cf Render.tar *
	gzip -f Render.tar


clean:
	rm -f obj/*.o render renderPyro renderWisp renderStamp renderFsim

