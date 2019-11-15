#include "../pch.h"
#include "../µº»ÎÕ∑/texfile.h"
#include <DirectXTex.h>
#include <memory>
#include <wincodec.h>
#include <wil/com.h>
#include <wil/resource.h>

wil::com_ptr<IWICImagingFactory> WIC_Factory;

using namespace ktexlib::v3;
constexpr inline bool check_4n(unsigned int val)
{
	return !(val & 0xFFFFFFF8u);
}

constexpr inline unsigned int next_4n(unsigned int val)
{
	return (val & 0xFFFFFFF8u) + 4;
}

//#define _CRT_SECURE_NO_WARNINGS
void ktexlib::v3::gen_bc3universal(char8_t * filename)
{
	/*unsigned char* processed = nullptr;
	if (pitch == 0)
		pitch = w;

	if ((!check_4n(w)) && (!check_4n(h)))
	{
		unsigned int w2 = next_4n(w);
		unsigned int h2 = next_4n(h);
		processed = new unsigned char[(size_t)w2 * (size_t)h2 * (size_t)4]{ 0 };
		for (unsigned int line = 0; line < h; line++)
			memcpy(
				processed + (size_t)line * (size_t)4,
				rgba_image + (size_t)line * (size_t)4,
				w * (size_t)4);
	}
	else
		processed = rgba_image;*/



}
#undef _CRT_SECURE_NO_WARNINGS

/*mipmap ktexlib::v3::Ktex::do_compress(const rgba_image& rgba, bool multithread, PixelFormat fmt)
{
	mipmap ret_val{};
	using namespace DirectX;

}*/
