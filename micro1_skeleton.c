#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


#include <math.h>
#include <stdio.h>
#include <float.h>


void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void sobelFiltering_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out);

int main()
{
 
	int height;
	int width;
	int channel;

	char command;

 	printf("Take a picture? (y/n)\n");
 	scanf("%c", &command);

 	if(command == 'n')
 		exit(1);


 	printf("Cheeze !\r\n");
	system("libcamera-still --width 640 --height 480 -o image.bmp");
 	
 	
 	unsigned char* imgIn = stbi_load("test_image.bmp", &width, &height, &channel, 3);


	unsigned char* imgOut_mirror = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_grayScale = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
	unsigned char* imgOut_sobelFiltering = (unsigned char*) malloc (sizeof(unsigned char)*3*640*480);
 	
	mirror_transform(imgIn, height, width, channel, imgOut_mirror);
	grayScale_transform(imgIn, height, width, channel, imgOut_grayScale);
	sobelFiltering_transform(imgOut_grayScale, height, width, channel, imgOut_sobelFiltering);


	stbi_write_bmp("image_mirror.bmp", width, height, channel, imgOut_mirror);
	stbi_write_bmp("image_grayScale.bmp", width, height, channel, imgOut_grayScale);
	stbi_write_bmp("image_sobelFiltering.bmp", width, height, channel, imgOut_sobelFiltering);
	
	stbi_image_free(imgIn);
	free(imgOut_mirror);
	free(imgOut_grayScale);
	free(imgOut_sobelFiltering);
 

	return 0;
}

void mirror_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int c = 0; c < channel; c++) {
				out[i * width * channel + j * channel + c] = in[i * width * channel + (width - j - 1) * channel + c];
			}
		}
	}
}

void grayScale_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int sum = 0;
			for (int c = 0; c < channel; c++) {
				sum += in[i * width * channel + j * channel + c];
			}
			out[i * width * channel + j * channel] = sum / 3;
			out[i * width * channel + j * channel + 1] = sum / 3;
			out[i * width * channel + j * channel + 2] = sum / 3;
		}
	}
}

void sobelFiltering_transform (unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	for (int j = 0; j < width - 2; j++) {
		for (int i = 0; i < height - 2; i++) {

			out[i * width * channel + j * channel] = abs(
				in[i * width * channel + j * channel] * -1 +
				in[i * width * channel + (j + 2) * channel] * 1 +
				in[(i + 1) * width * channel + j * channel] * -2 +
				in[(i + 1) * width * channel + (j + 2) * channel] * 2 +
				in[(i + 2) * width * channel + j * channel] * -1 +
				in[(i + 2) * width * channel + (j + 2) * channel] * 1) +
				abs(
					in[i * width * channel + j * channel] * 1 +
					in[i * width * channel + (j + 1) * channel] * 2 +
					in[i * width * channel + (j + 2) * channel] * 1 +
					in[(i + 2) * width * channel + j * channel] * -1 +
					in[(i + 2) * width * channel + (j + 1) * channel] * -2 +
					in[(i + 2) * width * channel + (j + 2) * channel] * -1);

			out[i * width * channel + j * channel + 1] = abs(
				in[i * width * channel + j * channel + 1] * -1 +
				in[i * width * channel + (j + 2) * channel + 1] * 1 +
				in[(i + 1) * width * channel + j * channel + 1] * -2 +
				in[(i + 1) * width * channel + (j + 2) * channel + 1] * 2 +
				in[(i + 2) * width * channel + j * channel + 1] * -1 +
				in[(i + 2) * width * channel + (j + 2) * channel + 1] * 1) +
				abs(
					in[i * width * channel + j * channel + 1] * 1 +
					in[i * width * channel + (j + 1) * channel + 1] * 2 +
					in[i * width * channel + (j + 2) * channel + 1] * 1 +
					in[(i + 2) * width * channel + j * channel + 1] * -1 +
					in[(i + 2) * width * channel + (j + 1) * channel + 1] * -2 +
					in[(i + 2) * width * channel + (j + 2) * channel + 1] * -1);

			out[i * width * channel + j * channel + 2] = abs(
				in[i * width * channel + j * channel + 2] * -1 +
				in[i * width * channel + (j + 2) * channel + 2] * 1 +
				in[(i + 1) * width * channel + j * channel + 2] * -2 +
				in[(i + 1) * width * channel + (j + 2) * channel + 2] * 2 +
				in[(i + 2) * width * channel + j * channel + 2] * -1 +
				in[(i + 2) * width * channel + (j + 2) * channel + 2] * 1) +
				abs(
					in[i * width * channel + j * channel + 2] * 1 +
					in[i * width * channel + (j + 1) * channel + 2] * 2 +
					in[i * width * channel + (j + 2) * channel + 2] * 1 +
					in[(i + 2) * width * channel + j * channel + 2] * -1 +
					in[(i + 2) * width * channel + (j + 1) * channel + 2] * -2 +
					in[(i + 2) * width * channel + (j + 2) * channel + 2] * -1);
		}
	}
}
