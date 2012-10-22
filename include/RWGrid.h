//*******************************************************************
//
//   Grid.h
//
// Full Grid Class
//
//*******************************************************************

#ifndef RW__GRID
#define RW_GRID


#include "BaseGrid.h"
#include <cstdio>
namespace lux{
	int writeGrid(BaseGrid<float> *grid,char *fname){
		FILE *file = fopen(fname,"w");
		if(file == NULL){
			fprintf(stderr,"FILE WRITE ERROR\n");
			return 0;
		}
		fprintf(file,"%d %d %d\n",grid->NX(),grid->NY(),grid->NZ());
		for(int k = 0; k < grid->NZ(); k++){
			for(int j = 0;j < grid->NY();j++){
				for(int i = 0;i < grid->NX();i++){
					fprintf(file,"%f ",grid->get(i,j,k));
				}
			}
			printf("WRITING GRID %.1f %%\r",(k * 100.0/grid->NZ()));
			fflush(stdout);
		}
		printf("\rWRITING GRID COMPLETE     \n");
		fclose(file);
		return 1;
		
	}
	
	
	int writeMaskGrid(BaseGrid<float> *grid,char *fname){
		FILE *file = fopen(fname,"w");
		if(file == NULL){
			fprintf(stderr,"FILE WRITE ERROR\n");
			return 0;
		}
		fprintf(file,"%d %d %d\n",grid->NX(),grid->NY(),grid->NZ());
		float value;
		for(int k = 0; k < grid->NZ(); k++){
			for(int j = 0;j < grid->NY();j++){
				for(int i = 0;i < grid->NX();i++){
					value = grid->get(i,j,k);
					if(value <= 0)
						value = 0;
					else
						value = 1;
					fprintf(file,"%f ",value);
				}
			}
			printf("WRITING MASKED GRID %.1f %%\r",(k * 100.0/grid->NZ()));
			fflush(stdout);
		}
		printf("\rWRITING MASKED GRID COMPLETE     \n");
		fclose(file);
		return 1;
		
	}
	
	
	
	
	
	
	
	int readGrid(BaseGrid<float> *grid,char *fname){
		FILE *file = fopen(fname,"r");
		if(file == NULL){
			fprintf(stderr,"FILE READ ERROR\n");
			return 0;
		}
		int NX,NY,NZ;
		fscanf(file,"%d %d %d",&NX,&NY,&NZ);
		if(NX != grid->NX() || NY != grid->NY() || NZ != grid->NZ()){
			fprintf(stderr,"GRID SIZES NOT EQUAL\n");
			return 0;
		}
		float x;
		for(int k = 0; k < grid->NZ(); k++){
			for(int j = 0;j < grid->NY();j++){
				for(int i = 0;i < grid->NX();i++){
					fscanf(file,"%f",&x);
					grid->set(i,j,k,x);
				}
			}
			printf("WRITING GRID %.1f %%\r",(k * 100.0/grid->NZ()));
			fflush(stdout);
		}
		printf("\rWRITING GRID COMPLETE     \n");
		fclose(file);
		return 1;
		
	}
	
	
	int readIntoGrid(BaseGrid<float> *grid,char *fname,int X,int Y, int Z){
		FILE *file = fopen(fname,"r");
		if(file == NULL){
			fprintf(stderr,"FILE READ ERROR\n");
			return 0;
		}
		int NX,NY,NZ;
		fscanf(file,"%d %d %d",&NX,&NY,&NZ);
		float x;
		for(int k = 0; k < NZ; k++){
			for(int j = 0;j < NY;j++){
				for(int i = 0;i < NX;i++){
					fscanf(file,"%f",&x);
					grid->set(i + X,j + Y,k + Z,x);
				}
			}
			printf("WRITING GRID %.1f %%\r",(k * 100.0/grid->NZ()));
			fflush(stdout);
		}
		printf("\rWRITING GRID COMPLETE     \n");
		fclose(file);
		return 1;
		
	}	
	
}

#endif


