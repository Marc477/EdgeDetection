
/*
	Marc-Antoine Desbiens
	Winter 2012
*/

#include "MImage.h"
#include <cstdio>
#include <cstdlib>
#include "math.h"

MImage::MImage(void)
{
	MXS = 0;
	MYS = 0;
	MZS = 0;
	MImgBuf=NULL;
}

MImage::MImage(int xs,int ys,int zs)
{
	MXS = xs;
	MYS = ys;
	MZS = zs;

	if(MXS>0 && MYS>0 && MZS>0)
		MAllocMemory(MXS,MYS,MZS);
	else
		MImgBuf=NULL;
}

MImage::MImage(const MImage &copy)
{
	MXS = 0;
	MYS = 0;
	MZS = 0;
	MImgBuf=NULL;

	if(copy.MIsEmpty()){
		return;
	}

	MAllocMemory(copy.MXS,copy.MYS,copy.MZS);

	for(int y=0;y<MYS;y++)
		for(int x=0;x<MXS;x++)
			MImgBuf[x][y]=copy.MImgBuf[x][y];

}

MImage::~MImage(void)
{
	MFreeMemory();

}

/* =================================================================================
====================================================================================
======================                    I/O                 ======================
====================================================================================
====================================================================================*/

/*
	Allocates an xs x ys x zs image buffer
*/
void MImage::MAllocMemory(int xs,int ys,int zs)
{
	MFreeMemory();
	if(xs<=0||ys<=0||zs<=0){
		printf("Error !! MImage::MAllocMemory\n");
		return;
	}

	MXS = xs;
	MYS = ys;
	MZS = zs;

	MImgBuf = new RGB*[xs];

	for(int i=0;i<xs;i++)
		MImgBuf[i] = new RGB[ys];
}

/*
	Free the image buffer
*/
void MImage::MFreeMemory(void)
{
	if(MImgBuf==NULL || MXS<=0 || MYS<=0 || MZS<=0){
		MXS=MYS=MZS=0;
	}else {
		for(int i=0;i<MXS;i++){
			delete []MImgBuf[i];
		}
		delete []MImgBuf;
		MImgBuf=NULL;
		MXS=MYS=MZS=0;
	}
}

/* =================================================================================
====================================================================================
======================         Various Functions              ======================
====================================================================================
====================================================================================*/

/*
	load a pgm/ppm image
*/
bool MImage::MLoadImage(const char *fileName)
{
	 int sizeX, sizeY, maxValue, nChannel=0;
	 bool isBinary=false;
	 FILE_FORMAT format;
	 char *line = new char[256];
	 char c;
	 ifstream imageFile;

	 imageFile.open(fileName, ios::binary);
	 imageFile.getline(line, 256);   //read magic number
 
	 switch(line[1])
	 {
		case '3':
			format = PPM_ASCII;
			nChannel = 3;
			isBinary = false;
			break;
		case '6':
			format = PPM_RAW;
			nChannel = 3;
			isBinary = true;
			break;
		case '2':
			format = PGM_ASCII;
			nChannel = 1;
			isBinary = false;
			break;
		case '5':
			format = PGM_RAW;
			nChannel = 1;
			isBinary = true;
			break;
		default:
			printf("Invalid file format for %s\n", fileName);
			break;
	 }
	 // skip comments
	 while(imageFile.peek() == '#')
	 {
		imageFile.getline(line, 256);
	 }

	 imageFile >> sizeX;
	 imageFile >> sizeY;
	 imageFile >> maxValue;

	 MAllocMemory(sizeX, sizeY, nChannel);

	 if(isBinary){
		 imageFile.get(c);
		 for(int j = 0; j < sizeY; j++){
			for(int i = 0; i < sizeX; i++){
				imageFile.get(c);
				MImgBuf[i][j].r = (unsigned char) c;
				if(format == PPM_RAW){
					imageFile.get(c);
					MImgBuf[i][j].g = (unsigned char) c;
					imageFile.get(c);
					MImgBuf[i][j].b = (unsigned char) c;
				}
				else{
					MImgBuf[i][j].g = MImgBuf[i][j].r;
					MImgBuf[i][j].b = MImgBuf[i][j].r;
				}
			}
		 }
	 }
	 else{
		 for(int j = 0; j < sizeY; j++){
			for(int i = 0; i < sizeX; i++){
				imageFile >> MImgBuf[i][j].r;
				if(format == PPM_ASCII){
					imageFile >> MImgBuf[i][j].g;
					imageFile >> MImgBuf[i][j].b;
				}
				else{
					MImgBuf[i][j].g = MImgBuf[i][j].r;
					MImgBuf[i][j].b = MImgBuf[i][j].r;
				}
			}
		 }
	 }

	 imageFile.close();
	 delete[] line;

	 printf("File %s opened successfully\n",fileName);
	 return true;
}

/*
	save a pgm/ppm image
*/
bool MImage::MSaveImage(const char *fileName, FILE_FORMAT ff)
{
	 string magicNumber;
	 bool isBinary;
	 ofstream imageFile;
	 
	 switch(ff)
	 {
		case PPM_ASCII:
			magicNumber = "P3";
			isBinary = false;
			break;
		case PPM_RAW:
			magicNumber = "P6";
			isBinary = true;
			break;
		case PGM_ASCII:
			magicNumber = "P2";
			isBinary = false;
			break;
		case PGM_RAW:
			magicNumber = "P5";
			isBinary = true;
			break;
		default:
			printf("Invalid file format for %s\n", fileName);
			break;
	 }

	 //Open and write header
	 imageFile.open(fileName, ios::binary);
	 imageFile << magicNumber << endl << MXS << " " << MYS << endl << 255 << endl;

	 //Copy channels if gray to color
	 if(ff == PPM_RAW && MZS == 1){
		 for(int i = 0; i < MXS; i++){
			 for(int j = 0; j < MYS; j++){
				MImgBuf[i][j].g = MImgBuf[i][j].r;
				MImgBuf[i][j].b = MImgBuf[i][j].r;
			 }
		 }
	 }

	 //Save
	 if(isBinary){
		 for(int j = 0; j < MYS; j++){
			for(int i = 0; i < MXS; i++){
				imageFile.put((unsigned char) (int) MImgBuf[i][j].r);
				if(ff == PPM_RAW){
					imageFile.put((unsigned char) (int) MImgBuf[i][j].g);
					imageFile.put((unsigned char) (int) MImgBuf[i][j].b);
				}
			}
		 }
	 }
	 else{
		 for(int j = 0; j < MYS; j++){
			for(int i = 0; i < MXS; i++){
				imageFile << (int) MImgBuf[i][j].r << " ";
				if(ff == PPM_ASCII){
					imageFile << (int) MImgBuf[i][j].g << " ";
					imageFile << (int) MImgBuf[i][j].b << " ";
				}
			}
			imageFile << endl;
		 }
	 }

	 imageFile.flush();
	 imageFile.close();

	printf("File %s saved successfully\n",fileName);
	return true;
}

/*
	Take the absolute value of every pixel
*/
void MImage::MAbs(void)
{

	for(int y=0;y<MYS;y++)
		for(int x=0;x<MXS;x++)
			for(int z=0;z<MZS;z++)
				MSetColor(fabs(MGetColor(x,y,z)),x,y,z);

}

/*
	Convert the current RGB image into a grayScale image
*/
void MImage::MRGBToGray(void)
{
	if(MZS<=1)
		return;

	for(int y=0;y<MYS;y++)
		for(int x=0;x<MXS;x++){
			float val = 0;
			for(int z=0;z<MZS;z++)
				val += MGetColor(x,y,z);
			val /= (float)MZS;

			MSetColor(val,x,y,0);
		}

	MZS = 1;
}

/*
	Apply the thresholding matrix to the current image.
	The result is a black and white image
*/
void MImage::MMatrixThreshold(const MImage &matrix)
{
	int Nx = matrix.MXSize();
	int Ny = matrix.MYSize();
	int x,y;
	float threshold;

	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			x = i % Nx;
			y = j % Ny;
			threshold = matrix.MGetColor(x,y);
			for(int k = 0; k < MZS; k++){
				if(MImgBuf[i][j][k] > threshold)
					MImgBuf[i][j][k] = MAX_VALUE;
				else
					MImgBuf[i][j][k] = 0;
			}
		}
	}
}



/* =================================================================================
====================================================================================
======================          Point Operators               ======================
====================================================================================
====================================================================================*/

/*
	Invert the intensity (or color) of every pixel:
		Intensity = 255 - oldIntensity
*/
void MImage::MInvert(void)
{
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			MImgBuf[i][j].r = MAX_VALUE - MImgBuf[i][j].r;
			MImgBuf[i][j].g = MAX_VALUE - MImgBuf[i][j].g;
			MImgBuf[i][j].b = MAX_VALUE - MImgBuf[i][j].b;
		}
	}
}

/*
	Threshold every pixel :
		all pixel with an intensity > 'tvalue' are set to 255.
		The other ones are set to 0.

	For a color image, repeat this process on each channel
*/
void MImage::MThreshold(float tvalue)
{
	float value, sum;
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			sum = 0;
			for(int k = 0; k < MZS; k++){
				sum += MGetColor(i,j,k);
			}
			if(sum/MZS < tvalue){
				value = 0;
			}
			else{
				value = MAX_VALUE;
			}
			MImgBuf[i][j].setAll(value);
		}
	}
}

/*
	Rescale the intensity level between 0 and 255
*/
void MImage::MRescale(void)
{
	RGB min, max;

	min.r = MImgBuf[0][0].r;
	min.g = MImgBuf[0][0].g;
	min.b = MImgBuf[0][0].b;
	max.r = MImgBuf[0][0].r;
	max.g = MImgBuf[0][0].g;
	max.b = MImgBuf[0][0].b;

	//Find min & max
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			for(int k = 0; k < 3; k++){
				if(MImgBuf[i][j].r > max.r) max.r = MImgBuf[i][j].r;
				if(MImgBuf[i][j].g > max.g) max.g = MImgBuf[i][j].g;
				if(MImgBuf[i][j].b > max.b) max.b = MImgBuf[i][j].b;
				if(MImgBuf[i][j].r < min.r) min.r = MImgBuf[i][j].r;
				if(MImgBuf[i][j].g < min.g) min.g = MImgBuf[i][j].g;
				if(MImgBuf[i][j].b < min.b) min.b = MImgBuf[i][j].b;
			}
		}
	}

	cout << "MAX: " << max.r << endl << "MIN: " << min.r << endl;

	//Rescale
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			MImgBuf[i][j].r = MAX_VALUE * ((MImgBuf[i][j].r - min.r)/(max.r - min.r));
			MImgBuf[i][j].g = MAX_VALUE * ((MImgBuf[i][j].g - min.g)/(max.g - min.g));
			MImgBuf[i][j].b = MAX_VALUE * ((MImgBuf[i][j].b - min.b)/(max.b - min.b));
		}
	}
}

/*
	Compute the average intensity value of the image.
	For a color image, the "channel" entry specifies which channel to process.
	For grayscale images, this entry is unuseful
*/
float MImage::MAverage(int channel)
{
	float sum = 0;
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			switch(channel){
				case 0: 
					sum += MImgBuf[i][j].r;
					break;
				case 1: 
					sum += MImgBuf[i][j].g;
					break;
				case 2: 
					sum += MImgBuf[i][j].b;
					break;
			}
		}
	}
	return sum/(MXS*MYS);
}

/*
	Compute the global contrast of the image
	For a color image, the "channel" entry specifies which channel to process.
	For grayscale images, this entry is unuseful
*/
float MImage::MContrast(int channel)
{
	float average, sum;
	sum = 0;
	average = MAverage(channel);
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			switch(channel){
				case 0: 
					sum += pow(MImgBuf[i][j].r - average,2);
					break;
				case 1: 
					sum += pow(MImgBuf[i][j].g - average,2);
					break;
				case 2: 
					sum += pow(MImgBuf[i][j].b - average,2);
					break;
			}
		}
	}
	return sqrt(sum/(MXS*MYS));
}

/* =================================================================================
====================================================================================
======================          Spatial filters               ======================
====================================================================================
====================================================================================*/

/*
	Compute the gradient of the current image.

		if dir = 0 then compute the X-gradient
		if dir = 1 then compute the Y-gradient

		Use of the central difference
*/
void MImage::MGradientFilter(int dir)
{
	float grad=0;
    MImage source(*this);

	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			for(int k = 0; k < MZS; k++){
				//Direction Y
				if(dir){
					if(j == 0)
						grad = source.MGetColor(i, j+1, k) - source.MGetColor(i, j, k);
					else if(j+1 == MYS)
						grad = source.MGetColor(i, j, k) - source.MGetColor(i, j-1, k);
					else
						grad = source.MGetColor(i, j+1, k)/2 - source.MGetColor(i, j-1, k)/2;
				}
				//Direction X
				else{
					if(i == 0)
						grad = source.MGetColor(i+1, j, k) - source.MGetColor(i, j, k);
					else if(i+1 == MXS)
						grad = source.MGetColor(i, j, k) - source.MGetColor(i-1, j, k);
					else
						grad = source.MGetColor(i+1, j, k)/2 - source.MGetColor(i-1, j, k)/2;
				}
				
				MSetColor(grad,i,j,k);
			}
		}
	}
}

/*
	Compute the euclidean norm of the gradient

		Norm  = sqrt[ Ux^2 + Uy^2 ]

		Use of the central difference
*/
void MImage::MNormGradientFilter(bool radical)
{
	float norm;

	MImage imgGradX(*this);
	imgGradX.MGradientFilter(0);

	MImage imgGradY(*this);
	imgGradY.MGradientFilter(1);

	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			for(int k = 0; k < MZS; k++){
				if(radical){
					norm = sqrt(pow(imgGradX.MGetColor(i, j, k),2) + pow(imgGradY.MGetColor(i, j, k),2));
				}
				else{
					norm = pow(imgGradX.MGetColor(i, j, k),2) + pow(imgGradY.MGetColor(i, j, k),2);
				}
				MSetColor(norm,i,j,k);
			}
		}
	}
}

/*
	Compute the laplacian of the current image i.e. comvolute the current image
	with the following mask

	| 0  1  0 |
	| 1 -4  1 |
	| 0  1  0 |
*/
void MImage::MLaplacianFilter(void)
{
	float sum;
	MImage source(*this);

	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			for(int k = 0; k < MZS; k++){
				sum=0;
				sum += source.MGetColorInImg2(i-1,j,k);
				sum += source.MGetColorInImg2(i+1,j,k);
				sum += source.MGetColorInImg2(i,j+1,k);
				sum += source.MGetColorInImg2(i,j-1,k);
				sum += -4*source.MGetColorInImg2(i,j,k);
				MSetColor(sum,i,j,k);
			}
		}
	}
}


/* =================================================================================
====================================================================================
======================            Feature extraction          ======================
====================================================================================
====================================================================================*/
/*
	Edge detection by thresholding the norm of the gradient of the current image
*/
void MImage::MEdgeDetec(float threshold)
{
	float dx,dy,magn;
	MImage source(*this);
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			for(int k = 0; k < MZS; k++){
				//delta x delta y
				if(j+1==MYS)
					dy = source.MGetColor(i, j, k) - source.MGetColor(i, j-1, k);
				else
					dy = source.MGetColor(i, j+1, k) - source.MGetColor(i, j, k);
				if(i+1==MXS)
					dx = source.MGetColor(i, j, k) - source.MGetColor(i-1, j, k);
				else
					dx = source.MGetColor(i+1, j, k) - source.MGetColor(i, j, k);
				
				//Magnitude
				magn = sqrt(pow(dx,2) + pow(dy,2));

				//Threshold
				if(magn > threshold){
					MSetColor(MAX_VALUE, i,j,k);
				}
				else{
					MSetColor(0, i,j,k);
				}
			}
		}
	}
}

/*
	Non max suppression algorithm for detecting edges
*/
void MImage::MNonMaxSupp(void)
{
	float theta;
	float pix,pix1,pix2;

	MImage imgGradX(*this);
	imgGradX.MGradientFilter(0);

	MImage imgGradY(*this);
	imgGradY.MGradientFilter(1);

	MNormGradientFilter(true);
	MRescale();

	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			for(int k = 0; k < MZS; k++){
				//Find orientation
				theta = atan(imgGradY.MGetColor(i, j, k) / imgGradX.MGetColor(i, j, k));
				
				pix = MGetColor(i, j, k);
				pix1=0;
				pix2=0;
				//Horizontal
				if(theta >= -PI/8 && theta < PI/8){
					pix1 = MGetColorInImg(i+1, j, k);
					pix2 = MGetColorInImg(i-1, j, k);
				}
				//Diagonal /
				else if(theta >= PI/8 && theta < 3*PI/8){
					pix1 = MGetColorInImg(i+1, j+1, k);
					pix2 = MGetColorInImg(i-1, j-1, k);
				}
				//Vertical
				else if(theta >= 3*PI/8 || theta < -3*PI/8){
					pix1 = MGetColorInImg(i, j+1, k);
					pix2 = MGetColorInImg(i, j-1, k);
				}
				//Diagonal \      //
				else if(theta >= -3*PI/8 && theta < -PI/8){
					pix1 = MGetColorInImg(i+1, j-1, k);
					pix2 = MGetColorInImg(i-1, j+1, k);
				}
				//Set to 0 if non-max
				if(pix < pix1 || pix < pix2){
					MSetColor(0,i,j,k);
				}
			}
		}
	}
}

/*
	Laplacian of Gaussian Zero crossing
*/
void MImage::MZeroCrossing(float threshold)
{
	MImage lapla(*this);
	lapla.MLaplacianFilter();

	//Test Zero crossing Threshold 4 neighbor
	for(int j = 0; j < MYS; j++){
		for(int i = 0; i < MXS; i++){
			for(int k = 0; k < MZS; k++){
				if (lapla.MGetColor(i,j,k) >= threshold){
					if (lapla.MGetColorInImg(i+1,j,k) < -threshold 
						|| lapla.MGetColorInImg(i-1,j,k) < -threshold
						|| lapla.MGetColorInImg(i,j+1,k) < -threshold
						|| lapla.MGetColorInImg(i,j-1,k) < -threshold 
						){
							MSetColor(MAX_VALUE,i,j,k);
					}
					else MSetColor(0,i,j,k);
				}
				else MSetColor(0,i,j,k);
			}
		}
	}
}

/* =================================================================================
====================================================================================
======================             Operators                  ======================
====================================================================================
====================================================================================*/
void MImage::operator= (const MImage &copy)
{
	if(copy.MIsEmpty()){
		MFreeMemory();
		return;
	}

	if(!MSameSize(copy))
		MAllocMemory(copy.MXS,copy.MYS,copy.MZS);

	for(int x=0;x<MXS;x++)
		for(int y=0;y<MYS;y++)
			MImgBuf[x][y]=copy.MImgBuf[x][y];

}

void MImage::operator-= (const MImage &img)
{
	if(!MSameSize(img))
		return;

	for(int y=0;y<MYS;y++)
		for(int x=0;x<MXS;x++)
			for(int z=0;z<MZS;z++)
				MSetColor(MGetColor(x,y,z)-img.MGetColor(x,y,z),x,y,z);

}

/* =================================================================================
====================================================================================
======================             Getters/ Setter            ======================
====================================================================================
====================================================================================*/

/*
	Return color at position only if inside borders
	else return 0
*/
float MImage::MGetColorInImg(int x, int y, int z) const{
	if(x>=0 && x<MXS && y>=0 && y<MYS && z>=0 && z<MZS)
		return MGetColor(x,y,z);
	else
		return 0;
}

/*
	Return color at position only if inside borders
	else return nearest border
*/
float MImage::MGetColorInImg2(int x, int y, int z) const{
	if(x<0) x=0;
	if(x>=MXS) x=MXS-1;
	if(y<0) y=0;
	if(y>=MYS) y=MYS-1;
	return MGetColor(x,y,z);
}
