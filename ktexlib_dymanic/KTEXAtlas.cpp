#include "KTEXAtlas.h"
#include <pugiconfig.hpp>
#include <pugixml.hpp>//vcpkg了解一下？
#include <algorithm>
using namespace std;
using namespace ktexlib::Atlas;
using namespace ktexlib;

unsigned short inline next2n(unsigned short a)
{
	return a + (a % 2);
}

bool inline operator<= (b_box l, b_box r)
{
	return (
		l.x <= r.w + r.x || l.y <= r.y + r.h
		);
}

ktexlib::Atlas::atlas::atlas(std::filesystem::path path, std::wstring outputname, vector<ktexlib::rgbav2> imgs)
{
	this->path = path.wstring();
	this->texfilename = outputname;
	for (auto img : imgs)
	{
		b_box tmp = 
		{
			img.w,
			img.h,
			0.5,
			0.5
		};
		bboxes.push_back(tmp);
	}
}

ktexlib::Atlas::atlas::atlas(std::filesystem::path path, std::wstring outputname, vector<ktexlib::mipmapv2> m)
{
	this->path = path.wstring();
	this->texfilename = outputname;
	for (auto mipmap : m)
	{
		b_box tmp = 
		{
			mipmap.w,
			mipmap.h,
			0.5,
			0.5
		};
		bboxes.push_back(tmp);
	}
}

void ktexlib::Atlas::atlas::xmlgen()
{
	using namespace pugi;
	xml_document document;
	auto Atlas = document.append_child(L"Atlas");
	auto Texture = Atlas.append_child(L"Texture");
	{
		auto rootfilename = Texture.append_attribute(L"filename");
		rootfilename.set_value(texfilename.c_str());
	}
	auto Elements = Atlas.append_child(L"Elements");
	for (auto bbox : bboxes)
	{
		double u1, v1, u2, v2 = 0.0;
		unsigned short w2, h2 = 0;//texture size

		w2 = next2n(bbox.w);
		h2 = next2n(bbox.h);
		
		double boff[2] = { bbox.w / w2,bbox.h / h2 };//border offset

		u1 = clamp(bbox.x / (double)w2 + boff[0], 0.0, 1.0);
		u2 = clamp(1.0 - bbox.x + bbox.w / (double)w2 - boff[0], 0.0, 1.0);

		v1 = clamp(bbox.y / (double)h2 + boff[1], 0.0, 1.0);
		v2 = clamp(1.0 - bbox.y + bbox.h / (double)h2 - boff[1], 0.0, 1.0);

		auto Element = Elements.append_child(L"Element");
		Element.append_attribute(L"name").set_value(texfilename.c_str());
		Element.append_attribute(L"u1").set_value(u1);
		Element.append_attribute(L"v1").set_value(v1);
		Element.append_attribute(L"u2").set_value(u2);
		Element.append_attribute(L"v2").set_value(v2);
	}
	document.save_file(path.c_str());
}
