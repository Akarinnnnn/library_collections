#define __API __declspec(dllexport)

#include "lodepng.h"
#include "./squish/squish.h"
#include "TEXFileOperation.h"
#include <Windows.h>
#include "resource1.h"
inline ktexlib::KTEXFileOperation::mipmapv2 夹带私货()
{
	HINSTANCE HI = (HINSTANCE)GetModuleHandleW(L"ktexlib_dymanic.dll");

	HBITMAP HBmp;
	BITMAP Bmp;
	HDC hdc = CreateICW(L"DISPLAY", NULL, NULL, NULL);
	srand((unsigned int)time(nullptr));
	//switch ((unsigned int)floor(rand() % 10))
	switch (1)
	{
	case(1):
		HBmp = LoadBitmapW(HI, MAKEINTRESOURCEW(IDB_BITMAP1));
		break;
	case(2):
		HBmp = LoadBitmapW(HI, MAKEINTRESOURCEW(IDB_BITMAP2));
		break;
	case(3):
		HBmp = LoadBitmapW(HI, MAKEINTRESOURCEW(IDB_BITMAP3));
		break;
	default:
		return ktexlib::KTEXFileOperation::mipmapv2{ 0 };
	}
	GetObjectW(HBmp, sizeof(BITMAP), &Bmp);
	size_t datasize = Bmp.bmHeight*Bmp.bmWidth * 4;
	auto bits = new char[datasize];
	if (!GetDIBits(hdc, HBmp, 1, Bmp.bmHeight, bits, NULL, DIB_RGB_COLORS))
	{
		return ktexlib::KTEXFileOperation::mipmapv2{ 0 };
	}
	ktexlib::KTEXFileOperation::mipmapv2 ret =
	{
		Bmp.bmWidth,
		Bmp.bmHeight,
		31
	};
	ret.size = datasize;
	ret.data = bits;
	return ret;
}


template<typename T>
inline bool ArrayCompare(T array1[], const T array2[], unsigned long long count)
{
	//UINT64 p1,p2 =(UINT64*)array1,(UINT64*)array2
	for (unsigned long long i = 0; i < count; i++)
	{
		if (*(array1 + i) != *(array2 + i))
		{
			return false;
		}
	}
	return true;
}

struct mipmapinfile
{
	unsigned short x = 0;
	unsigned short y = 0;
	unsigned short z = 0;
	unsigned int size = 0;
};

char* ReverseByByte(char* p, unsigned long long bytecount)//p强制转换就行
{
	char mid = 0;
	for (unsigned long long i = 0; i < (bytecount / 2); i++)
	{
		mid = *(p + (bytecount - i - 1));
		*(p + (bytecount - i - 1)) = *(p + i);
		*(p + i) = mid;
	}
	return p;
}

using namespace std;

inline void __fastcall parseheader(ktexlib::KTEXFileOperation::KTEXHeader header, ktexlib::KTEXFileOperation::KTEXInfo& info)
{
	info.flags			 = (header.firstblock & 0x000C0000) >>18;
	info.mipscount		 = (header.firstblock & 0x0003E000) >>13;
	info.texturetype	 = (ktexlib::KTEXFileOperation::textyp)((header.firstblock & 0x00001E00) >>9);
	info.pixelformat	 = (ktexlib::KTEXFileOperation::pixfrm)((header.firstblock & 0x000001F0) >>4);
	info.platform		 = (ktexlib::KTEXFileOperation::platfrm)(header.firstblock & 0x0000000F) ;
}

__API void ktexlib::KTEXFileOperation::KTEX::PushRGBA(ktexlib::KTEXFileOperation::RGBAv2 RGBA_array)
{
	this->RGBA_vectors.push_back(RGBA_array);
}

__API void ktexlib::KTEXFileOperation::KTEX::PushRGBA(RGBAv2 RGBA_array, unsigned int pitch)
{
	RGBA_array.pitch = pitch;
	this->RGBA_vectors.push_back(RGBA_array);
}

__API void ktexlib::KTEXFileOperation::KTEX::Convert()
{
	//生成mipmaps
	this->mipmaps.clear();
	for (auto img : RGBA_vectors)
	{
		mipmapv2 temp;
		temp.height = img.height;
		temp.width = img.width;
		temp.pitch = img.pitch;
		//上下翻转
		unsigned int* p_imgvec = (unsigned int*)img.data.data();
		for (unsigned short y = 0Ui16; y < img.height / 2Ui16; y++)
		{
			auto curline = (unsigned int*)p_imgvec + ((size_t)y * (size_t)img.width);
			auto tgtline = (unsigned int*)p_imgvec + ((size_t)(img.height - y - 1Ui16)*(size_t)img.width);
			for (unsigned short x = 0; x < img.width; x++)
			{
				unsigned int temp = *(tgtline + x);
				*(tgtline + x) = *(curline + x);
				*(curline + x) = temp;
			}
		}
		//转换
		char* data = nullptr;
		switch (Info.pixelformat)
		{
			using namespace squish;
		case(pixfrm::ARGB):
			temp.size = img.data.size();
			temp.data = (char*)img.data.data();
			break;
		case(pixfrm::DXT1):
			temp.size = GetStorageRequirements(img.width, img.height, kDxt1);
			data = new char[temp.size];
			CompressImage(img.data.data(), img.width, img.height, data, kDxt1);
			temp.data = data;
			break;
		case(pixfrm::DXT3):
			temp.size = GetStorageRequirements(img.width, img.height, kDxt3);
			data = new char[temp.size];
			CompressImage(img.data.data(), img.width, img.height, data, kDxt3);
			temp.data = data;
			break;
		case(pixfrm::DXT5):
			temp.size = GetStorageRequirements(img.width, img.height, kDxt5);
			data = new char[temp.size];
			CompressImage(img.data.data(), img.width, img.height, data, kDxt5);
			temp.data = data;
			break;
		default:
			throw std::invalid_argument("Wrong pixel format");
		}
		mipmaps.push_back(temp);
	}
	Info.mipscount = mipmaps.size();

	//生成第一数据块
	//必要检查
	if (Info.mipscount > 0x1F)
	{
		throw std::out_of_range("too much mipmaps, max 31.");
	}
	if (!(Info.mipscount == 0x1F))
	{
		auto si_huo = 夹带私货();
		if (si_huo.height != 0)
		{
			mipmaps.push_back(si_huo);
		}
	}

	Header.firstblock = 0xFFF00000;//保留，不排除未来官方会用
	Header.firstblock |= Info.flags << 18;
	Header.firstblock |= Info.mipscount << 13;
	Header.firstblock |= (unsigned int)Info.texturetype << 9;
	Header.firstblock |= (unsigned int)Info.pixelformat << 4;
	Header.firstblock |= (unsigned int)Info.platform;


	//写入文件
	ofstream file(output, ios::trunc | ios::binary);
	file.write((char*)(&this->Header), 8);//文件头(CC4+第一数据块)
	for (auto mipmap : this->mipmaps)//分别写入mipmaps
	{
		file.write((char*)(&mipmap), 6);
		file.write((char*)(&mipmap.size), 4);
		file.write(mipmap.data, mipmap.size);
	}
	file.close();
}

__API void ktexlib::KTEXFileOperation::KTEX::LoadKTEX(std::experimental::filesystem::path filepath)
{
	fstream file(filepath, ios::in | ios::binary);
	if (!file.is_open())
	{
		throw KTEXexception("failed to open specified file.", -1);
	}
	file.read((char*)(&this->Header), 8);
	if (Header.cc4 != 0x5845544B)
	{
		throw KTEXexception("Invalid KTEX file.", 1);
	}
	output = filepath.stem().wstring() + L".tex";
	parseheader(this->Header, this->Info);
	mipmaps.resize(Info.mipscount);
	for (unsigned int i = 0; i < Info.mipscount; i++)
	{
		mipmapv2& target = mipmaps[i];
		file.read((char*)(&target), 6);
		file.read((char*)(&target.size), 4);//估计是对齐问题
		auto temp = new char[target.size];
		file.read(temp, target.size);
		target.data = temp;
	}
	file.close();
}

__API ktexlib::KTEXFileOperation::mipmapv2 ktexlib::KTEXFileOperation::KTEX::GetMipmapByPitch(unsigned int pitch)
{
	for (auto a : this->mipmaps)
		if (a.pitch == pitch)
			return a;
	throw std::invalid_argument("no such mipmap");
}

__API ktexlib::KTEXFileOperation::mipmapv2 ktexlib::KTEXFileOperation::KTEX::GetMipmap(size_t order)
{
	return this->mipmaps[order];
}

__API ktexlib::KTEXFileOperation::RGBAv2 ktexlib::KTEXFileOperation::KTEX::GetImageFromMipmap(size_t order)
{
	auto& tmpmipmap = this->mipmaps[order];
	RGBAv2 tempRGBA =
	{
		tmpmipmap.width,
		tmpmipmap.height,
		tmpmipmap.pitch
	};
	if (Info.pixelformat == pixfrm::ARGB)
	{
		tempRGBA.data.reserve(tmpmipmap.size + 10);
		tempRGBA.data.assign(tmpmipmap.data, tmpmipmap.data + tmpmipmap.size + 1);
	}
	else
	{
		int flag = 0;
		switch (this->Info.pixelformat)
		{
		case(pixfrm::DXT5):
			flag = squish::kDxt5;
			break;
		case(pixfrm::DXT1):
			flag = squish::kDxt1;
			break;
		case(pixfrm::DXT3):
			flag = squish::kDxt3;
			break;
		default:
			throw KTEXexception("Invalid pixelformat", 2);
		}
		tempRGBA.data.resize(tmpmipmap.height * tmpmipmap.width * 4);
		squish::DecompressImage(tempRGBA.data.data(), tmpmipmap.width, tmpmipmap.height, tmpmipmap.data, flag);
	}
	return tempRGBA;
}

__API ktexlib::KTEXFileOperation::RGBAv2
ktexlib::KTEXFileOperation::KTEX::GetImageArray(unsigned int pitch)
{
	for (auto a : this->RGBA_vectors)
		if (a.pitch == pitch)
			return a;
	throw std::invalid_argument("no such image");
}

__API void ktexlib::KTEXFileOperation::KTEX::clear()
{
	this->mipmaps.clear();
	this->RGBA_vectors.clear();
	
}

__API ktexlib::KTEXFileOperation::KTEX::KTEX()
{
	this->mipmaps.reserve(5);
	this->RGBA_vectors.reserve(5);
}

__API ktexlib::KTEXFileOperation::KTEX::~KTEX()
{
	//do nothing.
}

__API void ktexlib::KTEXFileOperation::KTEX::operator+=(ktexlib::KTEXFileOperation::RGBAv2 src)
{
	this->RGBA_vectors.push_back(src);
}

__API ktexlib::KTEXFileOperation::RGBAv2* ktexlib::KTEXFileOperation::KTEX::operator[](int i)
{
	return RGBA_vectors.data() + i;
}

__API ktexlib::KTEXFileOperation::KTEX ktexlib::KTEXFileOperation::operator+(KTEX L, ktexlib::KTEXFileOperation::RGBAv2 R)
{
	KTEX temp(L);
	temp.PushRGBA(R);
	return temp;
}

__API void ktexlib::KTEXFileOperation::KTEX2PNG(KTEX target)
{

}

__API ktexlib::KTEXFileOperation::mipmapv2::~mipmapv2()
{
	if(this->data == nullptr)
		delete[] this->data;
	this->data = nullptr;
	this->size = 0;
}
