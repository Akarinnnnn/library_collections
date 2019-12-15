#include "../pch.h"
#include "../导入头/texfile.h"
#include <DirectXTex.h>
#include <memory>
#include <wincodec.h>
#include <wil/com.h>
#include <wil/resource.h>
#include <iostream>
#include <locale>
#include <fstream>

#define TO_PCHAR(x) reinterpret_cast<char*>(x)
#define TO_CONST_PCHAR(x) reinterpret_cast<const char*>(x)

using std::cout, std::endl;
//static std::codecvt<char16_t, char8_t, mbstate_t> wcsconv;
namespace wicobj
{
	extern wil::com_ptr<IWICImagingFactory> factory;
}

//static std::vector<wil::com_ptr<IWICBitmapDecoder>> decoders;
using namespace ktexlib::v3detail;
constexpr inline bool check_4n(unsigned int val)
{
	return !(val & 0xFFFFFFF8u);
}

constexpr inline unsigned int next_4n(unsigned int val)
{
	return (val & 0xFFFFFFF8u) + 4;
}

void ktexlib::v3::gen_bc3universal(const char8_t * filename)
{
	cout << std::hex;
	char16_t path[MAX_PATH]{ 0 };
	{
		mbstate_t mb;
		const char8_t* from_next = nullptr;
		char16_t* to_next = nullptr;
		std::use_facet<std::codecvt<char16_t, char8_t, mbstate_t>>(std::locale("chs")).in(mb,
			filename, filename + strlen(reinterpret_cast<const char*>(filename)), from_next,
			path, path + MAX_PATH, to_next
		);
	}

	gen_bc3universal((const wchar_t*)path);

}

bool operator== (GUID& l, GUID& r)
{
	//reinterpret_cast<>
	if (!(l.Data1 == r.Data1 &&
		l.Data2 == r.Data2 &&
		l.Data3 == r.Data3))
		return false;

#ifdef _AMD64_
	auto l4 = reinterpret_cast<uint64_t*>(l.Data4);
	auto r4 = reinterpret_cast<uint64_t*>(r.Data4);
	return l4 == r4;
#elif defined(_X86_)
	auto l4 = reinterpret_cast<uint32_t*>(l.Data4);
	auto r4 = reinterpret_cast<uint32_t*>(r.Data4);
	return (l4[0] == r4[0]) && (l4[1] == r4[1]);
#else
#error 不支持的架构
#endif
}


bool ktexlib::v3::gen_bc3universal(const char16_t * path)
{
	return gen_bc3universal((const wchar_t*)path);
}

bool ktexlib::v3::gen_bc3universal(const wchar_t* path)
{
	DirectX::ScratchImage bc3_images;
	{
		DirectX::ScratchImage src;
		DirectX::ScratchImage mipmap_chain;

		if (FAILED(DirectX::LoadFromWICFile(path, DirectX::WIC_FLAGS_NONE, nullptr, src))) return false;

		if (FAILED(DirectX::GenerateMipMaps(src.GetImages(), src.GetImageCount(),
			src.GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, mipmap_chain))) return false;

		if (FAILED(DirectX::Compress(mipmap_chain.GetImages(), mipmap_chain.GetImageCount(), mipmap_chain.GetMetadata(),
			DXGI_FORMAT_BC3_UNORM,
			DirectX::TEX_COMPRESS_PARALLEL, 1.0f, bc3_images))) return false;
	}
	auto dxmipmaps = bc3_images.GetImages();
	size_t mipscount = bc3_images.GetImageCount();

	ktexlib::v3detail::Ktex output;
	for (size_t i = 0; i < mipscount; i++)
	{
		const DirectX::Image& dx = dxmipmaps[i];
		size_t length = (dx.height * dx.rowPitch);
		Mipmap to_move(dx.width, dx.height, dx.rowPitch);
		to_move.data.assign(dx.pixels, dx.pixels + length);
		output.AddMipmap(std::move(to_move));
	}
	std::filesystem::path outfile(path);
	output.SaveFile(outfile.replace_extension(L".tex"));
	return true;

}

ktexlib::v3detail::Ktex ktexlib::v3::load_ktex(const char8_t* path)
{
	std::ifstream file;
	file.open(path);
	return load_ktex(path);
}

ktexlib::v3detail::Ktex ktexlib::v3::load_ktex(const wchar_t* path)
{
	std::ifstream file;
	file.open(path);
	return load_ktex(file);
}

ktexlib::v3detail::Ktex ktexlib::v3::load_ktex(std::ifstream& file)
{
	using namespace ktexlib::v3detail;
	Ktex ret;
	uint16_t mipscount = 0;
	{
		uint32_t v = 0;
		file.read(TO_PCHAR(&v), 4);
		if (v != 0x5845544BU) throw std::invalid_argument("不是有效的ktex文件");
		file.read(TO_PCHAR(&v), 4);
		ret.info.flags = (v & 0x000C0000) >> 18;
		ret.info.textureType	= (ktexlib::v3detail::textureType)	((v & 0x00001E00) >> 9);
		ret.info.pixelFormat	= (ktexlib::v3detail::PixelFormat)	((v & 0x000001F0) >> 4);
		ret.info.platform		= (ktexlib::v3detail::Platform)		(v & 0x0000000F);
		mipscount = (v & 0x0003E000) >> 13;
		//ret.mips.reserve(mipscount);
	}

	{
		Mipmap mip{ 0,0,0 };
		unsigned int mipdatasize = 0;
		for (uint16_t i = 0; i < mipscount; i++)
		{
			file.read(TO_PCHAR(&mip.width), 6);
			file.read(TO_PCHAR(&mipdatasize), 4);

			mip.data.resize(mipdatasize);
			file.read(TO_PCHAR(mip.data.data()), mipdatasize);

			ret.AddMipmap(std::move(mip));
		}
	}

	return ret;
}

void ktexlib::v3detail::Ktex::AddMipmap(const Mipmap& mipmap)
{
	mips.push_back(mipmap);
}

void ktexlib::v3detail::Ktex::AddMipmap(Mipmap&& mipmap)
{
	mips.push_back(std::move(mipmap));
}

inline ktexlib::v3detail::Mipmap::Mipmap(int32_t w, int32_t h, int32_t pitch) :width(w), height(h), pitch(pitch), data() {}

inline ktexlib::v3detail::Mipmap::Mipmap(const Mipmap& other) :Mipmap(other.width, other.height, other.pitch, data) {}

inline ktexlib::v3detail::Mipmap::Mipmap(Mipmap&& xval) noexcept :Mipmap(xval.width, xval.height, xval.pitch, std::move(xval.data))  {}

inline ktexlib::v3detail::Mipmap::Mipmap(const int32_t w, const int32_t h, const int32_t pitch, const std::vector<unsigned char>& data) :
	width(w), height(h), pitch(pitch)
{
	this->data = data;
}

inline ktexlib::v3detail::Mipmap::Mipmap(const int32_t w, const int32_t h, const int32_t pitch, std::vector<unsigned char>&& data) :
	width(w), height(h), pitch(pitch)
{
	this->data = std::move(data);
}
