class Matrix 
{
	private
		double** mat_;
		int n_, 
		int m_;

	public:
	
		Matrix(int n, int m);
		Matrix(char* stroka);
		Matrix(double** mat, int n, int m,);
		
		double** get_mat_data();
		int get_mat_row();
		int get_mat_col();
		double get_mat_elem(int i, int j);
		
		void oneD_to_twoD(double* mat1, double** mat2, int n, int m);
		void twoD_to_oneD(double **mat1, int n, int m, double* mat2);
		Matrix mat_mult(Matrix& mat1, Matrix& mat2);
		bool can_i_multiply(Matrix& matr1, Matrix& matr2);
};

char* matrix_to_string(Matrix& matrix);

