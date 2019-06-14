#include "Procesamiento.h"


String InputDir = "C:/Users/Genoma/Desktop/DICOM/imagenes/";
String Nombre = "Mamo MLO (I)_130420191110.jpg";

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

	// Mediana 7x7
	Mat mediana;
	medianBlur(src, mediana, 7);
	datos(mediana, "mediana");

	// Auenta el contraste local
	Mat contraste = Clahe(mediana);
	datos(contraste,"CLAHE");
	
	imwrite("C:/Users/Genoma/Desktop/DICOM/imagenes/mediana7/Result_" + Nombre, contraste);

	/*

	Mat prueba = Mat::eye(7, 7, CV_8UC1) *255;
	datos(prueba,"prueba");

	ofstream IMAGEN;
	IMAGEN.open("imagen.txt");

	for (int j = 0; j < prueba.rows; j++)
	{
		for (int i = 0; i < prueba.cols; i++)
		{
			IMAGEN << to_string(prueba.at<uint8_t>(j, i)) << " ";
			
		}
		IMAGEN << endl;
	}
	IMAGEN.close();

	*/

	waitKey(0);
	return 0;

}
