//#ifndef Procesamiento_H
//#define Procesamiento_H


#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;



Mat curva(Mat img, vector<cv::Point> &cont_musculo);
void datos(Mat image, string etapa);
void eliminacionPectoral(Mat &src);
Mat Clahe(Mat original);


Mat filtro_mediana_adaptativo(Mat src, int w_max);