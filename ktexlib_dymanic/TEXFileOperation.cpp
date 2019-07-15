#include "lodepng.h"
#include "./squish/squish.h"
#include "TEXFileOperation.h"
#include "resource1.h"
#include <filesystem>
#include "KTEXAtlas.h"
#ifdef _WIN32
#include <Windows.h>
inline ktexlib::KTEXFileOperation::RGBAv2 add_surpise()
{
	HINSTANCE HI = (HINSTANCE)GetModuleHandleW(L"ktexlib_dymanic.dll");
	HDC hdc = CreateCompatibleDC(GetDC(GetDesktopWindow()));
	HBITMAP hbmp;
	BITMAP bmp;
	char* rawdata = nullptr;
	ktexlib::KTEXFileOperation::RGBAv2 ret;
	srand((unsigned int)time(nullptr));
	switch ((unsigned int)floor(rand() % 21))
	//switch (1) //测试用
	{
	case(1):
		hbmp = (HBITMAP)LoadImageW(HI, L"#102",0,0,0,LR_CREATEDIBSECTION);
		break;
	case(2):
		hbmp = (HBITMAP)LoadImageW(HI, L"#103",0,0,0,LR_CREATEDIBSECTION);
		break;
	case(3):
		hbmp = (HBITMAP)LoadImageW(HI, L"#104",0,0,0,LR_CREATEDIBSECTION);
		break;
	default:
		return ktexlib::KTEXFileOperation::RGBAv2{ 0 };
	}
	GetObjectW(hbmp, sizeof(BITMAP), &bmp);
	if (hbmp != 0)
	{
		ret =
		{
			
			(unsigned short)(bmp.bmWidthBytes / bmp.bmBitsPixel * 8),
			(unsigned short)bmp.bmHeight,
			0x001FUi16,
		};
		{
			size_t size = (size_t)(bmp.bmHeight*bmp.bmWidthBytes / 3) * 4Ui64;
			switch (bmp.bmBitsPixel)
			{
			case 32:
				rawdata = new char[(size_t)bmp.bmHeight*(size_t)bmp.bmWidthBytes];
				memcpy_s(rawdata, (size_t)bmp.bmHeight*(size_t)bmp.bmWidthBytes, bmp.bmBits, (size_t)bmp.bmHeight*(size_t)bmp.bmWidthBytes);
				break;
			case 24:
				ret.data.resize(size);
				for (size_t i = 0; i < size/8; i++)
				{
					*(ret.data.data() + i)		= *((char*)bmp.bmBits + i);
					*(ret.data.data() + i + 1)	= *((char*)bmp.bmBits + i + 1);
					*(ret.data.data() + i + 2)	= *((char*)bmp.bmBits + i + 2);
					*(ret.data.data() + i + 3)	= 0x00Ui8;
				}
				break;
			default:
				return ktexlib::KTEXFileOperation::RGBAv2();
			}
		}

		DeleteObject(hbmp);
		return ret;
	}
	return ktexlib::KTEXFileOperation::RGBAv2();
}
#endif
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

inline void __fastcall parseheader(ktexlib::KTEXFileOperation::KTEXHeader& header, ktexlib::KTEXFileOperation::KTEXInfo& info)
{
	info.flags			 = (header.firstblock & 0x000C0000) >>18;
	info.mipscount		 = (header.firstblock & 0x0003E000) >>13;
	info.texturetype	 = (ktexlib::KTEXFileOperation::textyp)((header.firstblock & 0x00001E00) >>9);
	info.pixelformat	 = (ktexlib::KTEXFileOperation::pixfrm)((header.firstblock & 0x000001F0) >>4);
	info.platform		 = (ktexlib::KTEXFileOperation::platfrm)(header.firstblock & 0x0000000F) ;
}

__API void ktexlib::KTEXFileOperation::KTEX::PushRGBA(RGBAv2 tgt)
{
	tgt.pitch = RGBA_vectors.size() + 1;
	Info.mipscount++;
	this->RGBA_vectors.push_back(tgt);
}

__API void ktexlib::KTEXFileOperation::KTEX::PushRGBA(RGBAv2 RGBA_array, unsigned int pitch)
{
	RGBA_array.pitch = pitch;
	Info.mipscount++;
	this->RGBA_vectors.push_back(RGBA_array);
}

__API void ktexlib::KTEXFileOperation::KTEX::Convert()
{
	this->mipmaps.clear();
	//mipmap数量检查
	if (Info.mipscount > 0x1F)
	{
		throw std::out_of_range("too many mipmaps, max 31");//KTEX结构限制
	}
	//随机抽取幸运Windows用户夹带私货
#ifdef _WIN32
	if (!(Info.mipscount == 0x1F))
	{
		auto si_huo = add_surpise();
		if (si_huo.height != 0)
		{
			RGBA_vectors.push_back(si_huo);
		}
	}
#endif
	//生成mipmaps
	for (auto& img : RGBA_vectors)
	{
		mipmapv2 temp;
		temp.height = img.height;
		temp.width = img.width;
		temp.pitch = img.pitch;

		//上下翻转，针对OpenGL而言
		//不过应该只有OpenGL（PC）会用这个工具
		if (Info.platform == platfrm::opengl)
		{
			unsigned int* p_imgvec = (unsigned int*)img.data.data();
			for (unsigned short y = 0Ui16; y < img.height / 2Ui16; y++)
			{
				auto curline = (unsigned int*)p_imgvec + ((size_t)y * (size_t)img.width);
				auto tgtline = (unsigned int*)p_imgvec + (((size_t)img.height - (size_t)y - (size_t)1Ui16) * (size_t)img.width);
				for (unsigned short x = 0; x < img.width; x++)
				{
					unsigned int temp = *(tgtline + x);
					*(tgtline + x) = *(curline + x);
					*(curline + x) = temp;
				}
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

	//生成第一数据块
	Header.firstblock = 0xFFF00000;//保留，不排除未来官方会用
	Header.firstblock |= Info.flags << 18;
	Header.firstblock |= Info.mipscount << 13;
	Header.firstblock |= (unsigned int)Info.texturetype << 9;
	Header.firstblock |= (unsigned int)Info.pixelformat << 4;
	Header.firstblock |= (unsigned int)Info.platform;


	//写入文件
	ofstream file(output, ios::trunc | ios::binary);
	file.write((char*)(&this->Header), 8);//文件头(CC4+第一数据块)
	for (auto& mipmap : this->mipmaps)//分别写入mipmaps
	{
		file.write((char*)(&mipmap), 6);
		file.write((char*)(&mipmap.size), 4);
		file.write(mipmap.data, mipmap.size);
	}
	file.close();

	//生成XML
	Atlas::atlas xmlop(filesystem::path(output).replace_extension(".xml").wstring(), mipmaps);
	xmlop.xmlgen();
}

__API void ktexlib::KTEXFileOperation::KTEX::LoadKTEX(std::filesystem::path filepath)
{
	fstream file(filepath, ios::in | ios::binary);
	if (!file.is_open())
	{
		throw ktex_exception("failed to open specified file.", -1);
	}
	file.read((char*)(&this->Header), 8);
	if (Header.cc4 != 0x5845544B)
	{
		throw ktex_exception("not a KTEX", 1);
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
	for (auto& a : this->mipmaps)
		if (a.pitch == pitch)
			return a;
	throw std::invalid_argument("no mipmap has specified pitch");
}

__API ktexlib::KTEXFileOperation::mipmapv2 ktexlib::KTEXFileOperation::KTEX::GetMipmap(unsigned int order)
{
	return this->mipmaps[order];
}

__API ktexlib::KTEXFileOperation::RGBAv2 ktexlib::KTEXFileOperation::KTEX::GetImageFromMipmap(unsigned int order)
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
		tempRGBA.data.reserve((size_t)tmpmipmap.size + 10Ui64);
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
			throw ktex_exception("Invalid Pixel format", 2);
		}
		tempRGBA.data.resize((size_t)tmpmipmap.height * (size_t)tmpmipmap.width * 4Ui64);
		squish::DecompressImage(tempRGBA.data.data(), tmpmipmap.width, tmpmipmap.height, tmpmipmap.data, flag);
	}
	return tempRGBA;
}

__API ktexlib::KTEXFileOperation::RGBAv2
ktexlib::KTEXFileOperation::KTEX::GetImageArray(unsigned int pitch)
{
	for (auto& a : this->RGBA_vectors)
		if (a.pitch == pitch)
			return a;
	throw std::invalid_argument("no mipmap has specified pitch");
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
	Info.mipscount++;
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

__API ktexlib::KTEXFileOperation::mipmapv2::~mipmapv2()
{
	if(this->data == nullptr)
		delete[] this->data;
	this->data = nullptr;
	this->size = 0;
}
