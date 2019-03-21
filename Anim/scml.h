#pragma once
#include <string>
#include <vector>
namespace Anim
{
	namespace SCML//klei特化
	{
		class file
		{
		public:
			unsigned int id = 0;
			unsigned int w, h = 0;
			float x, y = 0.0F;
			std::wstring name;
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
		public:
			unsigned int id = 0;
			std::wstring name;
			unsigned int folder, file = 0;
			unsigned int z_index = 0;
			float x, y = 0.0f;
			float angle = 0.0f;//0~360°,将转换到弧度制
			float xscale = 0.0f;
			float yscale = 0.0f;
		};

		struct object
		{
		public:
			unsigned int id = 0;
			unsigned int folder,file = 0;
			float x, y = 0.0f;
			float xscale,yscale = 0.0f;
		};

		class key
		{
		public:
			std::vector<object> objects;
			unsigned int id = 0;
		};

		class key_mainline
		{
		public:
			std::vector<object_ref> object_refs;
			unsigned int id = 0;
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
			std::vector<key_mainline> keys;
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