#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Matrix_.h"

Matrix::Matrix(int n, int m)
{
	this->n_ = n;
	this->m_ = m;
	
	this->mat_ = new double*[n];
	
	for(int i=0;i<n;i++)
	{
		this->mat_[i] = new double[m];
		
		for(int j=0;j<m;j++)
		{
			this->mat_[i][j] = 0;
		}
	}
}

Matrix::Matrix(char* stroka)
{
	int n;
	int m;
	double** matr;
	
	char symbol;
	int i=0;
	
	while((stroka[i] != ' ') || (stroka[i] != '\n') || (stroka[i] != '\0'))
	{
		i++;
	}
	
	char* cmd = (char*)malloc(i);
	
	int k=i;
	int j=0;

	for(j=0; j < i; j++)
	{
		cmd[j] = stroka[j];
	}
	
	n = atoi(cmd);
	free(cmd);
	if(n<0)
	{
		perror("\nWrong rows count brUh\n");
	}
	
	while((stroka[i] != ' ') || (stroka[i] != '\n') || (stroka[i] != '\0'))
	{
		i++;
	}
	
	char* comd = (char*)malloc(i-k);
	
	for (int j=0; j<i-k; j++)
	{
		comd[j] = stroka[j];
	}
	
	m = atoi(comd);
	free(comd);
	
	if(m<0)
	{
		perror("\nWrong cols count bruH\n");
	}
	
	double* massiv = new double[n*m];
	int p = 0;
		
		while(i < sizeof(stroka))
		{	
			int k = i;
			int dlina = 0;
			
			while(stroka[k] != ' ')
			{
				dlina++;
				k++;
			}
			
			k=i;
			
			char* suda = new char[dlina];
			int m = 0;
			
			while(m < dlina)
			{
				suda[m] = stroka[k];
				m++;
				k++;
			}
			
			massiv[p] = atoi(suda);
			free(suda);
			p++;
		}
	
	Matrix matrica(n, m);
	
	this->n_ = n;
	this->m_ = m;
	this->mat_ = new double*[n];

	for(int q=0; q<n; q++)
	{
		this->mat_[q] = new double[m];
	}
	
	oneD_to_twoD(massiv, this->mat_, n_ , m_);
	
	free(massiv);
}

double** oneD_to_twoD(double* mat1, double** mat2, int n, int m)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			mat2[i][j] = mat1[i*j +j];
		}
	}

	return mat2;
}

double* twoD_to_oneD(double** mat1, int n, int m, double* mat2)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			mat2[i*j +j] = mat1[i][j];
		}
	}

	return mat2;
}

Matrix::Matrix(double** mat, int n, int m)
{	
	this->n_ = n;
	this->m_ = m;

	for(int i=0; i<n; i++)
	{
		for(int j=0; j<m; j++)
		{
			this->mat_[i][j] = mat[i][j];
		}
	}
	
}

double Matrix::get_mat_elem(int i, int j)
{
	return this->mat_[i][j];
}

double** Matrix::get_mat_data()
{
	return this->mat_;
}

Matrix* mat_mult(Matrix& mat1, Matrix& mat2)
{

	if(mat1.get_mat_col() != mat2.get_mat_row()) {
		printf("\nCan not mult those two\n");
		return NULL;
	}

	int new_n = mat1.get_mat_row();
	int new_m = mat2.get_mat_col();
	int mid_size = mat1.get_mat_col();

	double** new_Mat = new double*[new_n];
	for(int i = 0; i < new_n; i++) 
	{
		new_Mat[i] = new double[new_m];

		for(int j = 0; j < new_m; j++) 
		{
			new_Mat[i][j] = 0;
			for(int k = 0; k < mid_size; k++) 
			{
				new_Mat[i][j] += mat1.get_mat_elem(i,k)*mat2.get_mat_elem(k,j);
			}
		}
	}
	Matrix* matrichka = new Matrix(new_Mat, new_n, new_m);
	
	return matrichka;
}

bool can_i_multiply(Matrix& matr1, Matrix& matr2)
{
	return (matr1.get_mat_col() == matr2.get_mat_row());
}

char* matrix_to_string(Matrix& matrix)
{
	double* massiv = new double[matrix.get_mat_row()*matrix.get_mat_col()];
	massiv = twoD_to_oneD(matrix.get_mat_data(), matrix.get_mat_row(), matrix.get_mat_col(), massiv);
	char** massivchik = new char*[sizeof(massiv)];


	
	for(int i=0; i<sizeof(massiv); i++)
	{
		massivchik[i] = new char[3];
		sprintf(massivchik[i], "%f", massiv[i]);
	}

	char* mat_string = new char[sizeof(massiv)*3];

	for(int i=0; i<sizeof(massiv); i++)
	{
		strcat(mat_string, massivchik[i]);
		free(massivchik[i]);
	}
	
	free(massiv);
	free(massivchik);

	return mat_string;
}
