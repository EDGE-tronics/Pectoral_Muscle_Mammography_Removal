#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>

using namespace cv;
using namespace std;

Mat curva(Mat img, vector<cv::Point> &cont_musculo) {

	Mat mascara = Mat::zeros(img.rows, img.cols, CV_8U);

	Mat M = Mat_<double>(cont_musculo.size(), 4);
	Mat I = Mat_<double>(cont_musculo.size(), 1);
	int count = 0;
	for (int i = 0; i < cont_musculo.size(); i++) {
	
		double x = (cont_musculo[i].x - img.cols / 2) / double(img.cols);
		M.at<double>(count, 0) = pow(x,3);
		M.at<double>(count, 1) = pow(x, 2);
		M.at<double>(count, 2) = x;
		M.at<double>(count, 3) = 1;

		I.at<double>(count, 0) = cont_musculo[i].y;
		count++;
				
	}

	SVD s(M);
	Mat q;
	s.backSubst(I, q);
	Mat background(img.rows, img.cols, CV_8UC1);
	for (int j = 0; j < img.cols; j++)
	{
		double x = (j - img.cols / 2) / double(img.cols);
		double Y_curva = q.at<double>(0, 0)*pow(x, 3) + q.at<double>(1, 0)*pow(x, 2) + q.at<double>(2, 0)*x + q.at<double>(3, 0);
		
		for (int i = 0; i < img.rows; i++)
		{
			if (Y_curva > i) {
			
				mascara.at<uchar>(i, j) = 255;
			}
		}
	}
	

	return mascara;
}



void datos(Mat image, string etapa)
{
	
	double min;
	double max;
	minMaxIdx(image, &min, &max);
	Scalar mean, dev;
	meanStdDev(image, mean, dev);
	float M = mean.val[0];
	float D = dev.val[0];
	cout << ">>>>>>> " << etapa << ": Minimo = " << min << "  Maximo = " << max << " MEDIA = " << M << " Desviacion = " << D << " Tamano: " << image.rows << " , " << image.cols << endl << endl;
	namedWindow(etapa, CV_WINDOW_KEEPRATIO);
	

	imshow(etapa, image);
	
}


String InputDir = "C:/Users/Genoma/Desktop/DICOM/imagenes/";
String Nombre = "20587664_f4b2d377f43ba0bd_MG_R_ML_ANON.jpg";

int main()
{

	Mat src = imread(InputDir + Nombre, CV_LOAD_IMAGE_GRAYSCALE);

	if (!src.data)                              // Check for invalid input
	{
		cout << "No se puedo abrir la imagen" << std::endl;
		waitKey(0);
		return -1;
	}

	datos(src, "Original");

	bool derecha = false;
	int der = 0;
	int izq = 0;

	// determinar de que lado esta el pecho y reflejar
	for (int i = 0; i < src.cols / 2; i++) {
		for (int k = 0; k < src.rows / 2; k++) {
			if (src.at<uchar>(k, i) > 0) {
				izq++;
				//binaria.at<uchar>(k, i) = 0;
			}
			if (src.at<uchar>(k, src.cols - i - 1) > 0) {
				der++;
				//binaria.at<uchar>(k, binaria.cols - i - 1) = 0;
			}
		}
	}
	if (der > izq) {
		derecha = true;
	}
	if (!derecha) {
		flip(src,src,1);
	}

	Mat original = src.clone();


	Mat binaria;
	threshold(src, binaria, 0, 255, THRESH_BINARY | CV_THRESH_OTSU);
	//datos(binaria, "binaria");
	
	vector<vector<cv::Point>> contours;

	cv::findContours(
		binaria,
		contours,
		cv::RETR_EXTERNAL,
		cv::CHAIN_APPROX_SIMPLE
	);

	int indice = 0;
	double area = 0;
	double area_max = 0;
	for ( int i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i]);
		if (area > area_max) {
			area_max = area;
			indice = i;
		}
	}

	binaria = Mat::zeros(binaria.rows, binaria.cols, CV_8U);
	drawContours(binaria, contours,indice, Scalar(255), FILLED, 8);
	Rect tejido = boundingRect(contours[indice]);
	Mat roi = src(tejido);
	Mat bin_roi = binaria(tejido);

	

	//datos(binaria, "binaria2");
	//datos(roi, "roi");

	int x = 0;
	int y = 0;
	Mat region;
	Rect musculo;
	int tam = 7;
	vector<int>Xv(tam);
	vector<int>Yv(tam);

	for (int i = 0; i < tam; i++) {
		for (x = 0; x < bin_roi.cols - 1; x++) {
			if (bin_roi.at<uchar>(i, x) > 0) {
				Xv[i] = x;
				break;
			}
		}
	}
	sort(Xv.begin(), Xv.end(), greater<int>());
	x = Xv[(tam + 1) / 2];

	for (int i = 0; i < tam; i++) {
		for (y = bin_roi.rows-1; y > 0 ; y--) {
			if (bin_roi.at<uchar>(y, bin_roi.cols - 1) > 0) {
				Yv[i] = y;
				break;
			}
		}
	}
	sort(Yv.begin(), Yv.end(), greater<int>());
	y = Yv[(tam+1)/2];

	cout << " X: " << x << " Y: " << y << " Yv: "<< Yv[5] <<endl;

	musculo = Rect(x, 0, roi.cols -1 -x , y);
	region = roi(musculo);
	rectangle(roi, Point(x,0), Point(bin_roi.cols - 1,y), Scalar(255), 2, 8, 0);


	//datos(bin_roi, "roi2");
	//datos(roi, "encerrado");
	
	//datos(region,"region");
	int luminancia = 0;
	int lum_max = 0;
	int area_triangulo = 0;

	Mat region_eq;
	equalizeHist(region, region_eq);

	for (int i = 0; i < region.cols; i++) {
		for (int k = 0; k < region.rows; k++)
		{
			//if (k < ( (region.rows-1) / (region.cols-1)*i))
			
				luminancia += region_eq.at<uchar>(k, i);
				area_triangulo++;

				if (region.at<uchar>(k, i) > lum_max) {
					lum_max = region_eq.at<uchar>(k, i);
				}
				//////////////
				if (k > ((region.rows - 1) / (region.cols - 1)*i)) {
					region_eq.at<uchar>(k, i) = 0;
				}
			///////////////////
		}
	}

	luminancia /= area_triangulo;

	//datos(region_eq, "region_comprobacion");

	Mat detec;
	int umbral = (lum_max + luminancia) / 2;
	threshold(region_eq, detec, umbral, 255, THRESH_BINARY);
	//datos(detec,"deteccion");

	Mat detec_morph;
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(13, 13));
	morphologyEx(detec, detec_morph, MORPH_OPEN, kernel);

	//datos(detec_morph,"apertura");
	
	
	vector<vector<cv::Point>> cont_musculo;

	cv::findContours(
		detec_morph,
		cont_musculo,
		cv::RETR_EXTERNAL,
		cv::CHAIN_APPROX_SIMPLE
	);
	
	area = 0;
	area_max = 0;
	for (int i = 0; i < cont_musculo.size(); i++)
	{
		area = contourArea(cont_musculo[i]);
		if (area > area_max) {
			area_max = area;
			indice = i;
		}
	}

	detec_morph = Mat::zeros(detec_morph.rows, detec_morph.cols, CV_8U);
	Mat mascara = curva(detec_morph, cont_musculo[indice]);
	//datos(mascara, "MASCARA");



	detec_morph = Mat::zeros(detec_morph.rows, detec_morph.cols, CV_8U);
	//drawContours(detec_morph, cont_musculo, indice, Scalar(255), FILLED, 8);


	for (int i = 0; i < mascara.cols; i++) {
		for (int k = 0; k < mascara.rows; k++) {

			if (mascara.at<uchar>(k, i) > 0) {
				original.at<uchar>(k+tejido.y, i+ tejido.x + musculo.x) = 0;
			}
		}
	}

	if (!derecha) {
		flip(original, original, 1);
	}

	datos(original, "RESULTADO");
	imwrite("C:/Users/Genoma/Desktop/DICOM/imagenes/resultados_3er/Result_"+Nombre, original);

	waitKey(0);
	return 0;

}
