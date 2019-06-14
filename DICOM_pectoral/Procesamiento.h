#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H


#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>      
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;



Mat curva(Mat img, vector<cv::Point> &cont_musculo);		//Ajuste por minimos cuadrados de la linea del borde del musculo, mediante descomposicion en valores singulares
void datos(Mat image, string etapa);						// Visualizar la imagen y parametros de la misma
void eliminacionPectoral(Mat &src);							// Elimina el musculo pectoral de la imagen "src"
Mat Clahe(Mat original);									// Aumento del contraste local mediante el metodo CLAHE


#endif // PROCESAMIENTO_H