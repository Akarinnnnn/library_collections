#include "..\\pch.h"
#include "Xml.hpp"

//cnm我决定内嵌一个pugixml
#include "..\thirdparty\pugixml.hpp"

using namespace std::string_literals;
using pugi::xml_node;
using pugi::xpath_node;
using std::filesystem::path;

std::wstring ToString(const char* str)
{
	std::wostringstream o;
	o << str;
	return o.str();
}

void KleiAnim::AnimBin2XML(std::filesystem::path binary, std::filesystem::path xmlpath)
{
	Binary::AnimationReader bin(binary);
	//static_cast不行,dymanic_cast没有虚函数不能用
	AnimBin2XML((Common::AnimationBase&)(bin), xmlpath);
}

void KleiAnim::AnimBin2XML(Binary::AnimationReader& binary, std::filesystem::path xmlpath)
{
	pugi::xml_document doc;
	auto anims = doc.append_child("Anims");
	unsigned int frame_idx = 0;

	for (auto& animation : binary)
	{
		auto xanim = anims.append_child("anim");//xml anim

		xanim.append_attribute("name").set_value(ToString(animation.name.c_str()).c_str());
		xanim.append_attribute("root").set_value(ToString(binary.de_hash(animation.rootsym_hash).c_str()).c_str());
		xanim.append_attribute("numframes").set_value(animation.frames.size());
		xanim.append_attribute("framerate").set_value(animation.frame_rate);


		for (auto& frame : animation.frames)
		{
			auto xf = xanim.append_child("frame");//xml frame
			frame_idx++;

			xf.append_attribute("idx").set_value(frame_idx);
			xf.append_attribute("x").set_value(frame.x);
			xf.append_attribute("y").set_value(frame.y);
			xf.append_attribute("w").set_value(frame.w);
			xf.append_attribute("h").set_value(frame.h);

			if (frame.events.size() != 0)
				for (auto event : frame.events)
					xf.append_child("event").set_value(binary.de_hash(event.name_hash).c_str());

			for (auto& element : frame.elements)
			{
				auto xelem = xf.append_child("element");//xml element

				xelem.append_attribute("name").set_value(binary.de_hash(element.name_hash).c_str());
				xelem.append_attribute("layername").set_value(binary.de_hash(element.layer_hash).c_str());
				xelem.append_attribute("frame").set_value(frame_idx);
				xelem.append_attribute("z_index").set_value(element.z);
				xelem.append_attribute("m_a").set_value(element.a);
				xelem.append_attribute("m_b").set_value(element.b);
				xelem.append_attribute("m_c").set_value(element.c);
				xelem.append_attribute("m_d").set_value(element.d);
				xelem.append_attribute("m_tx").set_value(element.tx);
				xelem.append_attribute("m_ty").set_value(element.ty);
			}
		}
	}

	doc.save_file(xmlpath.c_str(), "  ", 1u, pugi::encoding_utf8);
}

void KleiAnim::AnimBin2XML(Common::AnimationBase& binary, std::filesystem::path xmlpath)
{
	Binary::AnimationReader reader(binary);
	AnimBin2XML(reader, xmlpath);
}

void KleiAnim::BuildBin2XML(std::filesystem::path binary, std::filesystem::path xmlpath)
{
	Binary::BuildReader bin(binary);
	BuildBin2XML(bin, xmlpath);
}

void KleiAnim::BuildBin2XML(Common::BuildBase& binary, std::filesystem::path xmlpath)
{
	Binary::BuildReader bin(binary);
	BuildBin2XML(bin, xmlpath);
}

void KleiAnim::BuildBin2XML(Binary::BuildReader& binary, std::filesystem::path xmlpath)
{
	pugi::xml_document doc;
	auto xroot = doc.append_child("Build");
	xroot.append_attribute("name").set_value(ToString(binary.name().c_str()).c_str());

	for (auto& sym : binary)
	{
		auto xsym = xroot.append_child("Symbol");
		xsym.append_attribute("name").set_value(binary.de_hash(sym.name_hash).c_str());
		
		for (auto& frame : sym.frames)
		{
			auto xframe = xsym.append_child("Frame");
			xframe.append_attribute("framenum").set_value(frame.frame_number);
			xframe.append_attribute("duration").set_value(frame.duration);
			//xframe.append_attribute("image").set_value(frame.) //build.bin中没有对应的信息
			xframe.append_attribute("x").set_value(frame.x);
			xframe.append_attribute("y").set_value(frame.y);
			xframe.append_attribute("w").set_value(frame.w);
			xframe.append_attribute("h").set_value(frame.h);
		}
	}

	doc.save_file(xmlpath.c_str(), "  ", 1u, pugi::encoding_utf8);
}

void build2bin(pugi::xml_document& document, path&& outfile)
{

}

void anim2bin(pugi::xml_document& document, path&& outfile)
{

}

void KleiAnim::XML2Bin(std::filesystem::path xmlpath, std::filesystem::path outdir)
{
	using namespace std::string_literals;
	using pugi::xml_node;
	using pugi::xpath_node;

	pugi::xml_document doc;
	doc.load_file(xmlpath.c_str());

	auto first = doc.first_child();

	const char* name = first.name();

	if (name == "Anims"s)
	{
		anim2bin(doc, outdir / L"anim.bin");
		return;
	}

	if (name == "Build"s)
	{
		build2bin(doc, outdir / L"build.bin");
		return;
	}

	throw std::invalid_argument("这可能是个假的xml文档，路径：" + outdir.string());
}
