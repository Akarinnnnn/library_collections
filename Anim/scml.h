#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <pugiconfig.hpp>
#include <pugixml.hpp>
namespace Anim
{
	namespace SCML
	{
		class scmlReader
		{
		public:
			scmlReader(std::wstring path);
			~scmlReader();

		private:
			pugi::xml_document scmldocument;
		};
		class file
		{
		public:
			unsigned int id = 0;
			unsigned int w, h = 0;
			float x, y = 0.0;
			std::wstring name;
			std::filesystem::path file;
		};
		class folder
		{
		public:
			std::vector<file> files;
			std::wstring name;
			unsigned int id;
		};
		class animation
		{
		public:
			std::wstring name;
			unsigned int id;
			unsigned int length;
		};
		class entity
		{
		public:
			std::wstring name;
			animation mainline;
			std::vector<animation> animations;
			unsigned int id;
		};
	}
}