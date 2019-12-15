#include "../pch.h"
#include "../../DirectXTex/DirectXTex.h"
#include "../导入头/texfile.h"
#include "wil/result.h"
//#include <wincodecsdk.h>
//#include <cmath>

using namespace ktexlib::v3detail;
using DirectX::Image;

Mipmap ktexlib::v3detail::convert(const RgbaImage& image, bool multithread, PixelFormat fmt)
{
	DXGI_FORMAT dxgi_fmt;

	switch (fmt)
	{
	case ktexlib::v3detail::PixelFormat::unknown:
		throw std::invalid_argument("fmt == PixelFormat::unknown");
		break;
	case ktexlib::v3detail::PixelFormat::dxt1:
		dxgi_fmt = DXGI_FORMAT_BC1_UNORM;
		break;
	case ktexlib::v3detail::PixelFormat::dxt3:
		dxgi_fmt = DXGI_FORMAT_D16_UNORM;
		break;
	case ktexlib::v3detail::PixelFormat::dxt5:
		dxgi_fmt = DXGI_FORMAT_BC3_UNORM;
		break;
	case ktexlib::v3detail::PixelFormat::rgba:
		return image;
		break;
	case ktexlib::v3detail::PixelFormat::r8g8b8:
		//throw std::invalid_argument("fmt == PixelFormat::r8g8b8");
	{
		//dxgi_fmt = DXGI_FORMAT_R8G8B8
		Mipmap retmip{image.width,image.height,image.pitch};
		auto size = image.data.size();
		retmip.data.reserve(size / 4 * 3);
		for (size_t i = 0; i < size; i++)
			if (!(i & 3u))
				retmip.data.push_back(image.data[i]);
		return retmip;
	}
		break;
	default:
		throw std::invalid_argument("所选格式不在可选范围内");
		break;
	}


	Image in_dx{ 
		image.width,image.height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		image.pitch,image.height,
		const_cast<unsigned char*>(image.data.data()) 
	};
	DirectX::ScratchImage out;

	DirectX::ScratchImage in;
	in.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, image.width, image.height, 0, 1);
	memcpy_s(in.GetImage(0, 0, 0)->pixels, (size_t)image.pitch * (size_t)4 * (size_t)image.height, image.data.data(), image.data.size());

	if(multithread)
	{
		THROW_IF_FAILED_MSG(DirectX::Compress(*in.GetImage(0, 1, 0), dxgi_fmt, DirectX::TEX_COMPRESS_PARALLEL, 1, out),"压缩失败");
	}
	else
	{
		in.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, image.width, image.height, 0, 1);
		THROW_IF_FAILED_MSG(DirectX::Compress(*in.GetImage(0, 1, 0), dxgi_fmt, 0, 1, out), "压缩失败");

	}

	Mipmap ret_val
	{
		image.width,image.height,image.pitch,
		std::vector<unsigned char>
		(out.GetPixels(),out.GetPixels() + out.GetPixelsSize())
	};
	return ret_val;
}