
/*	
	Marc-Antoine Desbiens
	Winter 2012
*/

#ifndef IMAGE_CLASS_H__
#define IMAGE_CLASS_H__

#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <time.h>
using namespace std;

#define MIN(i,j)  ((i)<(j)?(i):(j))
#define MAX(i,j)  ((i)>(j)?(i):(j))
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

const float MAX_VALUE = 255;
const float PI = 3.14159265;

enum FILE_FORMAT {PGM_RAW,PGM_ASCII,PPM_RAW,PPM_ASCII}; //P5,P2,P6,P3

struct RGB {
	float r; /* RED   Note: r contains the intensity value when MImage contains a gray-scale image */
	float g; /* GREEN Note: 0 for a gray-scale image */
	float b; /* BLUE  Note: 0 for a gray-scale image */
public:
	void setAll(float a){r=g=b=a;};
	float& operator[](const int k){
		if(k==0) return r;
		if(k==1) return g;
		if(k==2) return b;
	}
};

class MImage{

		int MXS;			/* number of columns */
		int MYS;			/* number of rows */
		int MZS;			/* number of channels (1 for grayscale, 3 for color) */

		RGB **MImgBuf; /* buffer containing the image */

	public:
		/* Constructors/destructor */
		MImage(void);       /* default constructor */
		MImage(int xs,int ys,int zs);
		MImage(const MImage &copy); /* copy constructor */
		~MImage(void);

		/* Various functions */
		int MXSize(void) const{return MXS;};
		int MYSize(void) const{return MYS;};
		int MZSize(void) const{return MZS;};
		void MSetColor(float val, int x, int y) {MImgBuf[x][y].r=val; if(MZS>1){MImgBuf[x][y].g=val;MImgBuf[x][y].b=val;}};
		void MSetColor(RGB color, int x, int y){MImgBuf[x][y].r=color.r; MImgBuf[x][y].g=color.g; MImgBuf[x][y].b=color.b;};
		void MSetColor(float val, int x, int y, int z) {if(z==0) MImgBuf[x][y].r=val; else if(z==1) MImgBuf[x][y].g=val; else MImgBuf[x][y].b=val;};
		float MGetColor(int x, int y) const {return MImgBuf[x][y].r;};
		float MGetColor(int x, int y, int z) const {if(z==0) return MImgBuf[x][y].r; else if(z==1) return MImgBuf[x][y].g; return MImgBuf[x][y].b;};
		float MGetColorInImg(int x, int y, int z) const;
		float MGetColorInImg2(int x, int y, int z) const;

		bool MIsEmpty(void) const{if(MImgBuf==NULL || MXS<=0 || MYS<=0 || MZS<=0) return true; return false;};
		bool MSameSize(const MImage &c){if(MXS==c.MXS && MYS==c.MYS && MZS==c.MZS) return true; return false;};

		/* I/O */
		void MAllocMemory(int xs,int ys,int zs);
		void MFreeMemory(void);

		bool MLoadImage(const char *imageName);
		bool MSaveImage(const char *imageName, FILE_FORMAT format);

		/* Point operations */
		void MInvert(void);
		void MThreshold(float value);
		void MRescale(void);
		float MAverage(int c);
		float MContrast(int c);
		void MRGBToGray(void);
		void MAbs(void);
		void MMatrixThreshold(const MImage &matrix);

		/* Spatial filters */
		void MGradientFilter(int dir);
		void MNormGradientFilter(bool radical);
		void MLaplacianFilter(void);
		void MCorrelationFilter(const MImage& corrImg);

		/* Feature extraction */ 
		void MEdgeDetec(float threshold);
		void MNonMaxSupp(void);
		void MZeroCrossing(float threshold);

		/* Operators */
		void operator= (const MImage &copy);
		void operator-= (const MImage &img);

};

#endif
