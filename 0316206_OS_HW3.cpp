#include "bmpReader.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <cstring>
//line 6
using namespace std;

#define MYRED	2
#define MYGREEN 1
#define MYBLUE	0

int imgWidth, imgHeight;
int BILINEAR_RATIO;
float ROTATION_ANGLE;
float SHEAR_VERTICAL_DEGREE;
float SHEAR_HORIZONTAL_DEGREE;
char *inputfile_name = "input.bmp";
char *outputfile_name[5] = { "0316206_output1.bmp", "0316206_output2.bmp", "0316206_output3.bmp", "0316206_output4.bmp", "0316206_output5.bmp"};
unsigned char *input_pic, *bilinear_pic, *rotation_pic, *shearVert_pic, *shearHori_pic, *mix_pic, *temp_pic;

unsigned char bilinear(unsigned char *pic, int w, int h, int color)
{
	int pixel = 0;
	int relevant_w = (int)(w / BILINEAR_RATIO);
	int relevant_h = (int)(h / BILINEAR_RATIO);
	int reference_points[4];

	reference_points[0] = pic[3 * (relevant_h * imgWidth + relevant_w) + color];
	reference_points[1] = pic[3 * (relevant_h * imgWidth + (relevant_w + 1)) + color];
	reference_points[2] = pic[3 * ((relevant_h + 1) * imgWidth + relevant_w) + color];
	reference_points[3] = pic[3 * ((relevant_h + 1) * imgWidth + (relevant_w + 1)) + color];

	double t = (w % BILINEAR_RATIO) / BILINEAR_RATIO;
	double u = (h % BILINEAR_RATIO) / BILINEAR_RATIO;
	double ut = u * t;
	int a = reference_points[2] - reference_points[0];
	int b = reference_points[1] - reference_points[0];
	int c = reference_points[0] - reference_points[1] - reference_points[2] + reference_points[3];
	int d = reference_points[0];
	
	pixel = (int)(a * t + b * u + c * ut + d);

	if (pixel < 0) pixel = 0;
	if (pixel > 255) pixel = 255;

	return (unsigned char)pixel;
}

unsigned char rotation(unsigned char *pic, int w, int h, int w_offset, int h_offset, int color)
{
	int pixel = 0;
	double angle = (double)ROTATION_ANGLE * M_PI / 180.0;
	double cos_theta = cos(angle);    
	double sin_theta = sin(angle);
	int relevant_w = w - w_offset;
	int relevant_h = h - h_offset;
	int trans_w = (int)(relevant_w * cos_theta - relevant_h * sin_theta) + w_offset;
	int trans_h = (int)(relevant_h * cos_theta + relevant_w * sin_theta) + h_offset;

	if (trans_w >= 0 && trans_w < w_offset * 2 && trans_h >= 0 && trans_h < h_offset * 2)
		pixel = pic[3 * (trans_h * 2 * w_offset + trans_w) + color];

	return (unsigned char)pixel;
}

unsigned char shear_vertical(unsigned char *pic, int w, int h, int border_w, int border_h, int color)
{
	int pixel = 0;
	int h_offset = (int)(border_w * SHEAR_VERTICAL_DEGREE / 2);
	int trans_w = w;
	int trans_h = (int)(h + w * SHEAR_VERTICAL_DEGREE) - h_offset;

	if (trans_h >= 0 && trans_h < border_h)
		pixel = pic[3 * (trans_h * border_w + trans_w) + color];

	return (unsigned char)pixel;
}

unsigned char shear_horizontal(unsigned char *pic, int w, int h, int border_w, int border_h, int color)
{
	int pixel = 0;
	int w_offset = (int)(border_h * SHEAR_HORIZONTAL_DEGREE / 2);
	int trans_w = (int)(w + h * SHEAR_HORIZONTAL_DEGREE) - w_offset;
	int trans_h = h;

	if (trans_w >= 0 && trans_w < border_w)
		pixel = pic[3 * (trans_h * border_w + trans_w) + color];

	return (unsigned char)pixel;
}
//line 92
typedef struct thre_para_1
{
    int w_offset;
    int h_offset;
    int nw;
    int nh;
    
} thre_pic;
void* pic2(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            rotation_pic[3 * (j * temp[0]->nw + i) + MYRED] = rotation(bilinear_pic, i, j, temp[0]->w_offset, temp[0]->h_offset, MYRED);
            rotation_pic[3 * (j * temp[0]->nw + i) + MYGREEN] = rotation(bilinear_pic, i, j, temp[0]->w_offset, temp[0]->h_offset, MYGREEN);
            rotation_pic[3 * (j * temp[0]->nw + i) + MYBLUE] = rotation(bilinear_pic, i, j, temp[0]->w_offset, temp[0]->h_offset, MYBLUE);
        }
    }
}
/*void* pic2a(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            rotation_pic[3 * (j * temp[0]->nw + i) + MYRED] = rotation(bilinear_pic, i, j, temp[0]->w_offset, temp[0]->h_offset, MYRED);
        }
    }
}
void* pic2b(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            rotation_pic[3 * (j * temp[0]->nw + i) + MYGREEN] = rotation(bilinear_pic, i, j, temp[0]->w_offset, temp[0]->h_offset, MYGREEN);
        }
    }
}
void* pic2c(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            rotation_pic[3 * (j * temp[0]->nw + i) + MYBLUE] = rotation(bilinear_pic, i, j, temp[0]->w_offset, temp[0]->h_offset, MYBLUE);
        }
    }
}*/
void* pic3(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            
            shearVert_pic[3 * (j * temp[0]->nw + i) + MYRED] = shear_vertical(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYRED);
            shearVert_pic[3 * (j * temp[0]->nw + i) + MYGREEN] = shear_vertical(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYGREEN);
            shearVert_pic[3 * (j * temp[0]->nw + i) + MYBLUE] = shear_vertical(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYBLUE);
        }
    }
}
/*void* pic3a(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            
            shearVert_pic[3 * (j * temp[0]->nw + i) + MYRED] = shear_vertical(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYRED);
        }
    }
}
void* pic3b(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            shearVert_pic[3 * (j * temp[0]->nw + i) + MYGREEN] = shear_vertical(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYGREEN);
        }
    }
}
void* pic3c(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            shearVert_pic[3 * (j * temp[0]->nw + i) + MYBLUE] = shear_vertical(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYBLUE);
        }
    }
}*/

void* pic4(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            shearHori_pic[3 * (j * temp[0]->nw + i) + MYRED] = shear_horizontal(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYRED);
            shearHori_pic[3 * (j * temp[0]->nw + i) + MYGREEN] = shear_horizontal(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYGREEN);
            shearHori_pic[3 * (j * temp[0]->nw + i) + MYBLUE] = shear_horizontal(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYBLUE);
            
        }
    }
}
/*void* pic4a(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            shearHori_pic[3 * (j * temp[0]->nw + i) + MYRED] = shear_horizontal(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYRED);
 
        }
    }
}
void* pic4b(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            shearHori_pic[3 * (j * temp[0]->nw + i) + MYGREEN] = shear_horizontal(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYGREEN);
        }
    }
}
void* pic4c(void* thr_da)
{
    thre_pic * temp[1];
    temp[0] = (thre_pic *) thr_da;
    for (int i = 0; i < temp[0]->nw; i++)
    {
        for (int j = 0; j < temp[0]->nh; j++)
        {
            shearHori_pic[3 * (j * temp[0]->nw + i) + MYBLUE] = shear_horizontal(bilinear_pic, i, j, temp[0]->nw, temp[0]->nh, MYBLUE);
        }
    }
}*/



//line 93
int main()
{
	BmpReader* bmpReader = new BmpReader();
	struct timeval  tv1, tv2;
	int nw, nh;
	
	// read cfg file
	FILE* cfg;
	cfg = fopen("cfg.txt", "r");
	fscanf(cfg, "%d", &BILINEAR_RATIO);
	fscanf(cfg, "%f", &ROTATION_ANGLE);
	fscanf(cfg, "%f", &SHEAR_VERTICAL_DEGREE);
	fscanf(cfg, "%f", &SHEAR_HORIZONTAL_DEGREE);

	// timing function 1
	gettimeofday(&tv1, NULL);
	input_pic = bmpReader->ReadBMP(inputfile_name, &imgWidth, &imgHeight);
	//line 110
	nw = (int)(imgWidth * BILINEAR_RATIO);
	nh = (int)(imgHeight * BILINEAR_RATIO);
	
	bilinear_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	rotation_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	shearVert_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	shearHori_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	mix_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));
	temp_pic = (unsigned char*)malloc(3 * nw * nh * sizeof(unsigned char));

	for (int i = 0; i < nw; i++)
	{
		for (int j = 0; j < nh; j++)
		{
			bilinear_pic[3 * (j * nw + i) + MYRED] = bilinear(input_pic, i, j, MYRED);
			bilinear_pic[3 * (j * nw + i) + MYGREEN] = bilinear(input_pic, i, j, MYGREEN);
			bilinear_pic[3 * (j * nw + i) + MYBLUE] = bilinear(input_pic, i, j, MYBLUE);
		}
	}

	int w_offset = (int)(nw / 2);
	int h_offset = (int)(nh / 2);
   
    pthread_t thread1,thread2,thread3;
    thre_pic rotat;
    rotat.h_offset = h_offset;
    rotat.w_offset = w_offset;
    rotat.nh = nh;
    rotat.nw = nw;
     pthread_create(&thread1, NULL ,pic2 , &rotat);
     pthread_create(&thread2, NULL ,pic3 , &rotat);
     pthread_create(&thread3, NULL ,pic4 , &rotat);
    /* pthread_t thread[9];
    pthread_create(&thread[0], NULL ,pic2a , &rotat);
     pthread_create(&thread[1], NULL ,pic2b , &rotat);
     pthread_create(&thread[2], NULL ,pic2c , &rotat);
     pthread_create(&thread[3], NULL ,pic3a , &rotat);
     pthread_create(&thread[4], NULL ,pic3b , &rotat);
    pthread_create(&thread[5], NULL ,pic3c , &rotat);
    pthread_create(&thread[6], NULL ,pic4a , &rotat);
    pthread_create(&thread[7], NULL ,pic4b , &rotat);
    pthread_create(&thread[8], NULL ,pic4c , &rotat);*/
	/*for (int i = 0; i < nw; i++)
	{
		for (int j = 0; j < nh; j++)
		{
			rotation_pic[3 * (j * nw + i) + MYRED] = rotation(bilinear_pic, i, j, w_offset, h_offset, MYRED);
			rotation_pic[3 * (j * nw + i) + MYGREEN] = rotation(bilinear_pic, i, j, w_offset, h_offset, MYGREEN);
			rotation_pic[3 * (j * nw + i) + MYBLUE] = rotation(bilinear_pic, i, j, w_offset, h_offset, MYBLUE);

			shearVert_pic[3 * (j * nw + i) + MYRED] = shear_vertical(bilinear_pic, i, j, nw, nh, MYRED);
			shearVert_pic[3 * (j * nw + i) + MYGREEN] = shear_vertical(bilinear_pic, i, j, nw, nh, MYGREEN);
			shearVert_pic[3 * (j * nw + i) + MYBLUE] = shear_vertical(bilinear_pic, i, j, nw, nh, MYBLUE);

			shearHori_pic[3 * (j * nw + i) + MYRED] = shear_horizontal(bilinear_pic, i, j, nw, nh, MYRED);
			shearHori_pic[3 * (j * nw + i) + MYGREEN] = shear_horizontal(bilinear_pic, i, j, nw, nh, MYGREEN);
			shearHori_pic[3 * (j * nw + i) + MYBLUE] = shear_horizontal(bilinear_pic, i, j, nw, nh, MYBLUE);
		}
	}*/

	for (int i = 0; i < nw; i++)
	{
		for (int j = 0; j < nh; j++)
		{
			temp_pic[3 * (j * nw + i) + MYRED] = rotation(shearVert_pic, i, j, w_offset, h_offset, MYRED);
			temp_pic[3 * (j * nw + i) + MYGREEN] = rotation(shearVert_pic, i, j, w_offset, h_offset, MYGREEN);
			temp_pic[3 * (j * nw + i) + MYBLUE] = rotation(shearVert_pic, i, j, w_offset, h_offset, MYBLUE);
		}
	}

	for (int i = 0; i < nw; i++)
	{
		for (int j = 0; j < nh; j++)
		{
			mix_pic[3 * (j * nw + i) + MYRED] = shear_horizontal(temp_pic, i, j, nw, nh, MYRED);
			mix_pic[3 * (j * nw + i) + MYGREEN] = shear_horizontal(temp_pic, i, j, nw, nh, MYGREEN);
			mix_pic[3 * (j * nw + i) + MYBLUE] = shear_horizontal(temp_pic, i, j, nw, nh, MYBLUE);
		}
	}

	bmpReader->WriteBMP(outputfile_name[0], nw, nh, bilinear_pic);
	bmpReader->WriteBMP(outputfile_name[1], nw, nh, rotation_pic);
	bmpReader->WriteBMP(outputfile_name[2], nw, nh, shearVert_pic);
	bmpReader->WriteBMP(outputfile_name[3], nw, nh, shearHori_pic);
	bmpReader->WriteBMP(outputfile_name[4], nw, nh, mix_pic);
	//line 177
	free(input_pic);
	free(bilinear_pic);
	free(rotation_pic);
	free(shearVert_pic);
	free(shearHori_pic);
	free(mix_pic);
	free(temp_pic);
	
	// timing function 2
	gettimeofday(&tv2, NULL);
	printf("0316206 %f seconds\n", (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec)); //(line 188) modify 0316001 to your student ID

	return 0;
}
