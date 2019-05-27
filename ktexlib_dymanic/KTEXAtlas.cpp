#include "KTEXAtlas.h"
#include <pugiconfig.hpp>
#include <pugixml.hpp>//vcpkg了解一下？
#include <algorithm>
using namespace std;
using namespace ktexlib::Atlas;
using namespace ktexlib;

bool __fastcall ispowerof2(unsigned short x)
{
	return !(x & (x - 1));
}

unsigned short inline next2pow(unsigned short x)
{
	x -= 1;
	x |= x >> 16;
	x |= x >> 8;
	x |= x >> 4;
	x |= x >> 2;
	x |= x >> 1;
	return x + 1;
}

bool inline operator<= (b_box l, b_box r)
{
	return (
		l.x <= r.w + r.x || l.y <= r.y + r.h
		);
}

ktexlib::Atlas::atlas::atlas(std::wstring output, vector<ktexlib::KTEXFileOperation::RGBAv2>& imgs)
{
	this->path = output;
	for (auto& img : imgs)
	{
		b_box tmp = 
		{
			img.width,
			img.height,
			0.5,
			0.5
		};
		bboxes.push_back(tmp);
	}
}

ktexlib::Atlas::atlas::atlas(std::wstring output, vector<ktexlib::KTEXFileOperation::mipmapv2>& m)
{
	this->path = output;
	for (auto& mipmap : m)
	{
		b_box tmp = 
		{
			mipmap.width,
			mipmap.height,
			0.5,//根据官方说法，这两个0.5是为了避免出现采样问题，可能跟klei的游戏引擎有关
			0.5
		};
		bboxes.push_back(tmp);
	}
}

void ktexlib::Atlas::atlas::xmlgen()
{
	using namespace pugi;
	xml_document document;
	wstring filename = path.filename().replace_extension(L".tex").wstring();// .c_str();

	auto Atlas = document.append_child(L"Atlas");
	auto Texture = Atlas.append_child(L"Texture");
	{
		auto rootfilename = Texture.append_attribute(L"filename");
		rootfilename.set_value(filename.c_str());
	}
	auto Elements = Atlas.append_child(L"Elements");
	for (auto&& bbox : bboxes)
	{
		float u1 = 0.0, v1 = 0.0, u2 = 0.0, v2 = 0.0;
		unsigned short w2 = 0, h2 = 0;//texture size

		//w2 = next2pow(bbox.w);
		//h2 = next2pow(bbox.h);
		
		float boffset[2] = { 0.5f / w2,0.5f / h2 };//border offset

		u1 = clamp<float>((bbox.x / w2) + boffset[0], 0.0f, 1.0f);
		u2 = clamp<float>(1.0f - bbox.x + bbox.w / w2 - boffset[0], 0.0f, 1.0f);

		v1 = clamp<float>(bbox.y / h2 + boffset[1], 0.0f, 1.0f);
		v2 = clamp<float>(1.0f - bbox.y + bbox.h / h2 - boffset[1], 0.0f, 1.0f);

		auto Element = Elements.append_child(L"Element");
		Element.append_attribute(L"name").set_value(filename.c_str());
		Element.append_attribute(L"u1").set_value(u1);
		Element.append_attribute(L"v1").set_value(v1);
		Element.append_attribute(L"u2").set_value(u2);
		Element.append_attribute(L"v2").set_value(v2);
	}
	document.save_file(path.c_str(), L"\t", 1, pugi::encoding_utf8);
}
