#include "../pch.h"
#include "DirectXTex.h"
#include "../导入头/texfile.h"
#include "wil/result.h"
//#include <wincodecsdk.h>
//#include <cmath>

using namespace ktexlib::v3;
using DirectX::Image;

//从DirectxTex导入
namespace
{
	__declspec(dllimport) extern HRESULT CompressBC(
		const Image& image,
		const Image& result,
		DWORD bcflags,
		DWORD srgb,
		float threshold);

	__declspec(dllimport) extern HRESULT CompressBC_Parallel(
		const Image& image,
		const Image& result,
		DWORD bcflags,
		DWORD srgb,
		float threshold);
}




mipmap ktexlib::v3::Ktex::do_compress(const rgba_image& image, bool multithread, PixelFormat fmt)
{
	DXGI_FORMAT dxgi_fmt;

	switch (fmt)
	{
	case ktexlib::v3::PixelFormat::unknown:
		throw std::invalid_argument("fmt == PixelFormat::unknown");
		break;
	case ktexlib::v3::PixelFormat::dxt1:
		dxgi_fmt = DXGI_FORMAT_BC1_UNORM;
		break;
	case ktexlib::v3::PixelFormat::dxt3:
		dxgi_fmt = DXGI_FORMAT_D16_UNORM;
		break;
	case ktexlib::v3::PixelFormat::dxt5:
		dxgi_fmt = DXGI_FORMAT_BC3_UNORM;
		break;
	case ktexlib::v3::PixelFormat::rgba:
		return image;
		break;
	case ktexlib::v3::PixelFormat::r8g8b8:
		throw std::invalid_argument("fmt == PixelFormat::r8g8b8");
		break;
	default:
		throw std::invalid_argument("所选格式不在可选范围内");
		break;
	}


	Image in_dx{ 
		image.width,image.height,
		DXGI_FORMAT_R8G8B8A8_UINT,
		image.pitch,image.height,
		const_cast<unsigned char*>(image.data.data()) 
	};
	DirectX::ScratchImage out;
	const Image* out_dx;
	THROW_IF_FAILED_MSG(out.Initialize2D(dxgi_fmt, image.width, image.height, 1, 1),"创建DX图像失败");
	out_dx = out.GetImage(0, 0, 1);

	if(multithread)
	{
		THROW_IF_FAILED_MSG(
			CompressBC(in_dx, *out_dx, DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_COMPRESS_SRGB, 1),
			"bc3压缩失败");
	}
	else
	{
		THROW_IF_FAILED_MSG(
			CompressBC_Parallel(in_dx, *out_dx, DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_COMPRESS_SRGB, 1),
			"bc3压缩失败");
	}

	mipmap ret_val
	{
		image.width,image.height,image.pitch,
		std::vector<unsigned char>
		(out.GetPixels(),out.GetPixels() + out.GetPixelsSize())
	};
	return ret_val;
}