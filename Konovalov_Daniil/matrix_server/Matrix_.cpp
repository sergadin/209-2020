#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Matrix_.h"

Matrix::Matrix(int n, int m)
{
	this->n_ = n;
	this->m_ = m;
	
	this->mat = new double[n];
	
	for(int i=0;i<n;i++)
	{
		this->mat[i] = new double[m];
		
		for(int j=0;j<m;j++)
		{
			this->mat[i][j] = 0;
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
	
	while (int j=0; j<i; j++)
	{
		cmd[j] = stroka[j];
	}
	
	n = atoi(cmd);
	free(cmd);
	if(n<0)
	{
		printf("\nWrong rows count brUh\n");
		return NULL;
	}
	
	while((stroka[i] != ' ') || (stroka[i] != '\n') || (stroka[i] != '\0'))
	{
		i++;
	}
	
	char* comd = (char*)malloc(i-k);
	
	while (int j=0; j<i-k; j++)
	{
		comd[j] = stroka[j];
	}
	
	m = atoi(comd);
	free(comd);
	
	if(m<0)
	{
		printf("\nWrong cols count bruH\n");
		return NULL;
	}
	
	double* massiv = (double)malloc(n*m);
	int p = 0;
		
		while(i < sizeof(stroka))
		{	
			int k = i;
			dlina = 0;
			
			while(stroka[k] != ' ')
			{
				dlina++;
				k++;
			}
			
			k=i;
			
			char* suda =(char)malloc(dlina);
			int m = 0;
			
			while(m < dlina)
			{
				suda[m] = stroka[k];
				m++;
				k++
			}
			
			massiv[p] = atoi(suda);
			free(suda);
			p++;
		}
	
	Matrix matrica(n, m);
	
	this->n_ = n;
	this->m_ = m;
	this->mat_ = new double[n][m];
	
	oneD_to_twoD(massiv, this->mat_, n_ , m_);
	
	free(massiv);
	
	return matrica;
}

void Matrix::oneD_to_twoD(double* mat1, double** mat2, int n, int m)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			mat2[i][j] = mat1[i*j +j];
		}
	}
}

void Matrix::twoD_to_oneD(double** mat1, int n, int m, double mat2)
{
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			mat2[i*j +j] = mat1[i][j];
		}
	}
}

Matrix::Matrix(double** mat, int n, int m)
{
	Matrix matr(n, m);
	
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<m; j++)
		{
			matr[i][j] = mat[i][j];
		}
	}
	
	return matr;
}

double Matrix::get_mat_elem(int i, int j)
{
	return this->mat_[i][j];
}

double** Matrix::get_mat_data()
{
	return this->data_;
}

Matrix::Matrix mat_mult(Matrix& mat1, Matrix& mat2)
{

	if(mat1.get_mat_col() != mat2.get_mat_row()) {
		printf("\nCan not mult those two\n");
		return NULL;
	}

	int new_n = mat1.get_mat_row();
	int new_m = mat2.get_mat_col();
	int mid_size = mat1.get_mat_col();

	int** new_Mat = new int*[new_n];
	for(int i = 0; i < new_n; i++) 
	{
		new_Mat[i] = new int[new_m];

		for(int j = 0; j < new_m; j++) 
		{
			new_Mat[i][j] = 0;
			for(int k = 0; k < mid_size; k++) 
			{
				new_Mat[i][j] += mat1[i][k]*mat2[k][j];
			}
		}
	}

	return Matrix(new_n, new_m, new_Mat);
}

bool Matrix::can_i_multiply(Matrix& matr1, Matrix& matr2)
{
	return (matr1.get_mat_col() == matr2.get_mat_row());
}

char* matrix_to_string(Matrix& matrix)
{
	double* massiv = new double[matrix.get_mat_row()*matrix.get_mat_col()];
	twoD_to_oneD(matrix.get_mat_data(), matrix.get_mat_row(), matrix.get_mat_col(), massiv);
	char* massivchik = new char[sizeof(massiv)*3];
	
	for(int i=0; i<sizeof(massiv)*3; i++)
	{
		massivchik[i] = to_string(massiv[i]);
	}
	
	free(massiv);
	
	return massivchik;
}
