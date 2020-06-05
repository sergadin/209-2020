#include<iostream>
#include<math.h>
#include<vector>

#define F_SIZE 10

typedef enum {h, v} Orientation;

class Ship {
	private:
		int row_, col_;
		int len_;
		Orientation ori_;
		int* status_;
	public:
		Ship(int row, int col, string ori, int len)
		{
			if((ori == h && row + len >= F_SIZE) || (ori == v && col + len >= F_SIZE))
			{
				printf("Coordinates is not correct");
				return -1;
			}
			if(!(0 <= row < F_SIZE) || !(0 <= col < F_SIZE) || (ori != h && ori != v))
			{
				printf("Coordinates is not correct");
				return -1;
			}
			row_ = row;
			col_ = col;
			ori_ = ori;
			len_ = len;
			status_ = new bool * len;
			status_ = {0};
		}
		bool wound(int row, int col)
		{
			if(ori = h)
			{
				for(int i = 0; i < len_; i++)
				{
					if(row_ == row && i + col_ == col)
					{
						status_[i] = 1;
						return true;
					}
				}
			}
			if(ori = v)
			{
				for(int i = 0; i < len_; i++)
				{
					if(col_ == col && i + row_ == row)
					{
						status_[i] = 1;
						return true;
					}
				}
			}
			return false;
		}
		bool kill() const
		{
			int i = 0;
			while(i < len_ && status_[i] == 1)
				i++;
			if(i == len_)
				return true
			return false
		}
		
		friend class Player;
}





















































