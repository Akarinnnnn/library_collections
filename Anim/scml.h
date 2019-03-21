#pragma once
#include <string>
#include <vector>
#include <filesystem>
namespace Anim
{
	namespace SCML//kleiÌØ»¯
	{
		class file
		{
		public:
			unsigned int id = 0;
			unsigned int w, h = 0;
			float x, y = 0.0F;
			std::wstring name;
			std::filesystem::path filepath;
		};
		class folder
		{
		public:
			std::vector<file> files;
			std::wstring name;
			unsigned int id=0;
		};
		struct object_ref
		{
			unsigned int id = 0;
			std::wstring name;
			float x, y = 0.0f;
			float angle = 0.0f;//radius angle
			float scale = 0.0f;
		};
		struct object
		{
			unsigned int id = 0;
			unsigned int folder,file = 0;
			float scale = 0.0f;
		};
		class key
		{
			std::vector<object> objects;
			unsigned int id;
		};
		class key_mainline
		{
		public:
			std::vector<object_ref> objects;
			unsigned int id;
		};
		class timeline
		{
		public:
			std::vector<key_mainline> keys;
			unsigned int id=0;
			unsigned int length=0;
		};
		class _mainline
		{
		public:
			std::vector<key> keys;
			std::wstring name;
			unsigned int length;//frame
		};
		class animation
		{
		public:
			_mainline mainline;
			std::vector<timeline> timelines;
			std::wstring name;
			unsigned int id=0;
			unsigned int length=0;//frame
		};
		class entity
		{
		public:
			std::wstring name;
			std::vector<animation> animations;
			unsigned int id=0;
		};
		struct scml
		{
			std::vector<folder> folder;
			std::vector<entity> entity;
		};
		scml ReadSCML(std::wstring path);
	}
}