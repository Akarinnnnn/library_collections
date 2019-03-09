#pragma once
namespace ktexlib
{
	namespace Atlas
	{
		struct b_box//boundry box
		{
			unsigned short width;
			unsigned short hwight;
			double x;//x offset
			double y;//y offset
		};
		template <typename T>
		/*class martix33
		{
		private:
			T data[3][3];
		public:
			martix33<T> operator* (martix33<T> R)
			{
				for (int i = 0; i < 9; i++)
					(*(this->data + i))*(*(R.data + i));
			}
			martix33<T> operator+ (martix33<T> R)
			{
				for (int i = 0; i < 9; i++)
					(*(this->data + i)) + (*(R.data + i));
			}
			martix33<T> operator-(martix33<T> R)
			{
				for (int i = 0; i < 9; i++)
					(*(this->data + i)) - (*(R.data + i))
			}
		};*/
	}
}