#include "Procesamiento.h"



Mat filtro_mediana_adaptativo(Mat src, int w_max) {

	if (w_max < 3) {

		cout << "ERROR: TAMANO MAXIMO DE VENTANA MENOR A 3";
		return src;
	}
	int nulo = (w_max + 1) / 2;
	int tam_vector = w_max * w_max;
	vector<int> pixeles(tam_vector);
	int tam_ventana = 5;
	int cont = 0;
	int a = (tam_ventana-1)/2;
	int xc = 0;
	int yc = 0;
	int x, y, i, j;
	Mat resultado(src.size(),CV_8UC1);

	for ( x = nulo; x < src.cols - nulo; x++) {
		for ( y = nulo; y < src.rows - nulo; y++) {

			for ( i = -a; i <= a; i++) {
				for ( j = -a; j <= a; j++) {

					//xc = x + i;
					//yc = y + j;
/*
					if (xc < 0) {
						xc = 1 - xc;
					}
					else if (xc >= src.cols) {
					
						xc = 2 * src.cols - xc - 1;
					
					}

					if (yc < 0) {
						yc = 1 - yc;
					}
					else if (yc >= src.rows) {

						yc = 2 * src.rows - yc - 1;

					}
*/
					pixeles[cont] = src.at<uchar>(y+j, x+i);
					cont++;
				}
			}
			sort(pixeles.begin(), pixeles.begin() + (tam_vector));
			///////////////
			//cout<<x<<","<<y<<endl;
			////////////////
			resultado.at<uchar>(y,x)=pixeles[(tam_vector-1)/2];
			cont = 0;
		}
	}

	return resultado;
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

	// Elimina el pectoral
	eliminacionPectoral(src);
	datos(src, "RESULTADO");
	imwrite("C:/Users/Genoma/Desktop/DICOM/imagenes/clahe/Result_"+Nombre, src);

	// Auenta el contraste local
	Mat contraste = Clahe(src);
	datos(contraste,"CLAHE");

	
	waitKey(0);
	return 0;

}
