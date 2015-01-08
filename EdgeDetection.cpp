
/*
	Marc-Antoine Desbiens
	Winter 2012
*/

#include "MImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* 
	Non-maximum suppression
*/
int main(int argc, char*argv[])
{
	MImage img,imgGrad,imgZero,imgCanny;
	
	if(argc<3){
		printf("Error!!!\n\nNot enough parameters\nEdgeDetection.exe [image file name] [threshold] \n\n");
		system("pause");
		exit(1);
	}

	/* Load input image */
	img.MLoadImage(argv[1]);
	
	//// Gradient Magnitude Edge Detection ////////////
	imgGrad = img;
	imgGrad.MEdgeDetec((float) atof(argv[2]));
	imgGrad.MSaveImage("outEdgeDetec.pgm",PGM_RAW);

	//// Zero Crossing Edge detection ////////////
	imgZero = img;
	imgZero.MZeroCrossing((float) atof(argv[2]));
	imgZero.MSaveImage("outZeroCrossing.pgm",PGM_RAW);
	
	/// Canny Edge Detection //////////	
	imgCanny = img;
	imgCanny.MNonMaxSupp();
	imgCanny.MSaveImage("outNonMax.pgm",PGM_RAW);

	/* Threshold */
	imgCanny.MThreshold((float) atof(argv[2]));
	imgCanny.MSaveImage("outNonMaxThr.pgm",PGM_RAW);

	return 0;
}
