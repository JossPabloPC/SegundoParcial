// Recocido_simulado.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>

float PI = 3.21416;

float energia(float posicion[], int dimension)
{
	float suma = 0.0;
	for (int i = 1; i <= dimension; i++)
		suma += (float)pow(posicion[i] - 3, 2) + 5;
	return suma;
}

float energia_Ackley(float posicion[], int dimension)
{//Dimension 4 
	float a = posicion[1];
	float b = posicion[2];
	float c = posicion[3];
	float d = posicion[4];

	float suma = 0.0;
	suma += -20 * exp(-0.2f * sqrt(0.25 * (a*a + b*b + c*c + d*d) ))  -exp(0.25 * (cos(2*PI*a) + cos(2 * PI * b) + cos(2 * PI * c) + cos(2 * PI * d))) + 20 + exp(1);
	return suma;
}

float energia_Bukin(float posicion[], int dimension)
{//Dimension 4 
	float a = posicion[1];
	float b = posicion[2];

	float res = 0.0;
	res = 100 * (sqrt(abs(b - 0.01f * (a * a))) + 0.01f * abs(a + 10));
	return res;
}

double energia_Cross(float posicion[], int dimension)
{//Dimension 2 
	float a = posicion[1];
	float b = posicion[2];

	return -0.0001f * powf(abs(sin(a) * sin(b) * exp2l(abs(100 - (sqrt(a * a + b * b) / PI)))) + 1, 0.1);

}

void nueva_solucion(float posicion[], float nueva[], int dimension, float min, float max)
{
	for (int i = 1; i <= dimension; i++)
	{
		float aleatorio = (float)rand() / (float)RAND_MAX;
		nueva[i] = min + abs(max - min) * aleatorio;
	
	}
}

void nueva_solucion_1(float posicion[], float nueva[], int dimension, float min, float max)
{
	for (int i = 1; i <= dimension; i++)
	{
		float aleatorio = (float)rand() / (float)RAND_MAX;
		if (aleatorio<0.5)
			nueva[i] = posicion[i] + 10 * aleatorio;
		else
			nueva[i] = posicion[i] - 10 * aleatorio;
	}
}

void nueva_solucion_2(float posicion[], float nueva[], int dimension, float min, float max, int temperatura, int temperatura_maxima)
{
	float variacion = (max - min) * temperatura/ temperatura_maxima;
	for (int i = 1; i <= dimension; i++)
	{
		float aleatorio = (float)rand() / (float)RAND_MAX;
		if (aleatorio < 0.5)
			nueva[i] = posicion[i] + variacion * aleatorio;
		else
			nueva[i] = posicion[i] - variacion * aleatorio;
		if (nueva[i] < min || nueva[i] > max)
		{
			aleatorio = (float)rand() / (float)RAND_MAX;
			nueva[i] = min + abs(max - min) * aleatorio;
		}
	}
}

void solucion_inicialnueva_solucion(float posicion[], int dimension, int min, int max)
{
	for (int i = 1; i <= dimension; i++)
	{
		float aleatorio = (float)rand() / (float)RAND_MAX;
		posicion[i] = min + abs(max - min) * aleatorio;
	}
}

void recocido_1_casi_Metropolis(int dimension, float min, float max, float & energia_min, float posicion_min[])
{
	float S_i[30];
	float S_j[30];
	float energia_i;
	float energia_j;
	solucion_inicialnueva_solucion(S_i, dimension, min, max);
	for (int temperatura = 1000; temperatura > 1; temperatura--)
	{
		for (int metropolis = 1; metropolis < 100; metropolis++)
		{
			energia_i = energia(S_i, dimension);
			nueva_solucion(S_i, S_j, dimension, min, max);
			energia_j = energia(S_j, dimension);
			if (energia_j < energia_i)
			{
				energia_min = energia_j;
				for (int j = 1; j <= dimension; j++)
					posicion_min[j] = S_j[j];
				for (int j = 1; j <= dimension; j++)
					S_i[j] = S_j[j];
			}
		}

	}
}

void recocido_2_casi_Metropolis(int dimension, float min, float max, float& energia_min, float posicion_min[])
{
	float S_i[30];
	float S_j[30];
	float energia_i;
	float energia_j;
	solucion_inicialnueva_solucion(S_i, dimension, min, max);
	for (int temperatura = 10000; temperatura > 1; temperatura--)
	{
		for (int metropolis = 1; metropolis < 1000; metropolis++)
		{
			energia_i = energia(S_i, dimension);
			nueva_solucion_1(S_i, S_j, dimension, min, max);
			energia_j = energia(S_j, dimension);
			if (energia_j < energia_i)
			{
				energia_min = energia_j;
				for (int j = 1; j <= dimension; j++)
					posicion_min[j] = S_j[j];
				for (int j = 1; j <= dimension; j++)
					S_i[j] = S_j[j];
			}
		}

	}
}

void recocido_simulado_Ackley(int dimension, float min, float max, float& energia_min, float posicion_min[])
{
	float	S_i[30];
	float	S_j[30];
	float	energia_i = FLT_MAX;
	float	energia_j = FLT_MAX;
	int		temp_temperatura;
	solucion_inicialnueva_solucion(S_i, dimension, min, max);
	for (float temperatura = 200000; temperatura > 1; temperatura/=1.618)
	{
		for (int metropolis = 1; metropolis < 50000; metropolis++)
		{
			energia_i = energia_Ackley(S_i, dimension);								//Función con valores Si
			nueva_solucion_2(S_i, S_j, dimension, min, max, temperatura, 200000); //Se obtienen valores Sj
			energia_j = energia_Ackley(S_j, dimension);								//Función con valores Sj
			if (energia_j < energia_i)
			{
				energia_i = energia_j;
				if (energia_j < energia_min)
				{
					for (int j = 1; j <= dimension; j++)
						posicion_min[j] = S_j[j];
					energia_min = energia_j;
				}
				for (int j = 1; j <= dimension; j++)
					S_i[j] = S_j[j];
			}
			else
			{
				float diferencia = energia_j - energia_i;
				float exponente = -diferencia / temperatura;
				float aleatorio = (float)rand() / (float)RAND_MAX;
				if (exp(exponente) > aleatorio)
				{
					for (int j = 1; j <= dimension; j++)
						S_i[j] = S_j[j];
					energia_i = energia_j;
				}
			}
			temp_temperatura = (int)temperatura;
			//printf("Temperatura: % f Energia: % f Energia minima: %f \n", temperatura, energia_i, energia_min);

		}

	}
}

void recocido_simulado_Buckin(int dimension, float min, float max, float& energia_min, float posicion_min[])
{
	float	S_i[30];
	float	S_j[30];
	float	energia_i = FLT_MAX;
	float	energia_j = FLT_MAX;
	int		temp_temperatura;
	solucion_inicialnueva_solucion(S_i, dimension, min, max);
	for (float temperatura = 200000; temperatura > 1; temperatura /= 1.618)
	{
		for (int metropolis = 1; metropolis < 50000; metropolis++)
		{
			energia_i = energia_Bukin(S_i, dimension);								//Función con valores Si
			nueva_solucion(S_i, S_j, dimension, min, max); //Se obtienen valores Sj
			energia_j = energia_Bukin(S_j, dimension);								//Función con valores Sj
			if (energia_j < energia_i)
			{
				energia_i = energia_j;
				if (energia_j < energia_min)
				{
					for (int j = 1; j <= dimension; j++)
						posicion_min[j] = S_j[j];
					energia_min = energia_j;
				}
				for (int j = 1; j <= dimension; j++)
					S_i[j] = S_j[j];
			}
			else
			{
				float diferencia = energia_j - energia_i;
				float exponente = -diferencia / temperatura;
				float aleatorio = (float)rand() / (float)RAND_MAX;
				if (exp(exponente) > aleatorio)
				{
					for (int j = 1; j <= dimension; j++)
						S_i[j] = S_j[j];
					energia_i = energia_j;
				}
			}
			temp_temperatura = (int)temperatura;
			//printf("Temperatura: % f Energia: % f Energia minima: %f \n", temperatura, energia_i, energia_min);

		}

	}
}

void recocido_simulado_Cross_in_Tray(int dimension, float min, float max, float& energia_min, float posicion_min[])
{
	float	S_i[30];
	float	S_j[30];
	long double	energia_i = DBL_MAX;
	long double energia_j = DBL_MAX;
	int		temp_temperatura;
	solucion_inicialnueva_solucion(S_i, dimension, min, max);
	for (float temperatura = 600000; temperatura > 1; temperatura /= 1.618)
	{
		for (int metropolis = 1; metropolis < 80000; metropolis++)
		{
			energia_i = energia_Cross(S_i, dimension);								//Función con valores Si
			nueva_solucion_2(S_i, S_j, dimension, min, max,temperatura, 200000); //Se obtienen valores Sj
			energia_j = energia_Cross(S_j, dimension);								//Función con valores Sj
			if (energia_j < energia_i)
			{
				energia_i = energia_j;
				if (energia_j < energia_min)
				{
					for (int j = 1; j <= dimension; j++)
						posicion_min[j] = S_j[j];
					energia_min = energia_j;
				}
				for (int j = 1; j <= dimension; j++)
					S_i[j] = S_j[j];
			}
			else
			{
				float diferencia = energia_j - energia_i;
				float exponente = -diferencia / temperatura;
				float aleatorio = (float)rand() / (float)RAND_MAX;
				if (exp(exponente) > aleatorio)
				{
					for (int j = 1; j <= dimension; j++)
						S_i[j] = S_j[j];
					energia_i = energia_j;
				}
			}
			temp_temperatura = (int)temperatura;
			//printf("Temperatura: % f Energia: % f X1: %f X2:%f\n", temperatura, energia_i, S_i[0], S_i[1]);
			//printf("Temperatura: % f Energia: % f Energia minima: %f  \n", temperatura, energia_i, energia_min);

		}

	}
}

int main()
{
	srand(1002);
	srand(4244);
	float energia_min = FLT_MAX;
	float posicion_min[30];
	//recocido_simulado_Ackley(4, -32.768, 32.768, energia_min, posicion_min);
	//recocido_simulado_Buckin(2, -15, 3, energia_min, posicion_min);
	recocido_simulado_Cross_in_Tray(2, -10, 10, energia_min, posicion_min);
}
