#pragma once
#include <string>
#include <vector>
namespace Anim
{
	namespace  SCML//klei特化
	{
		struct __declspec(dllexport) file
		{
		public:
			unsigned int id = 0;
			unsigned int w=0, h = 0;
			float x=0.0f, y = 0.0F;
			std::wstring name;
		};

		struct __declspec(dllexport) folder
		{
		public:
			std::vector<file> files;
			std::wstring name;
			unsigned int id=0;
		};
		
		struct __declspec(dllexport) object_ref
		{
		public:
			unsigned int id = 0;
			std::wstring name;
			unsigned int folder=0, file = 0;
			unsigned int z_index = 0;
			float x=0.0f, y = 0.0f;
			float angle = 0.0f;//0~360°,将转换到弧度制
			float xscale = 0.0f;
			float yscale = 0.0f;
		};

		struct __declspec(dllexport) object
		{
		public:
			unsigned int id = 0;
			unsigned int folder=0,file = 0;
			float x=0.0f, y = 0.0f;
			float xscale = 0.0f,yscale = 0.0f;
		};

		struct __declspec(dllexport) key
		{
		public:
			std::vector<object> objects;
			unsigned int id = 0;
		};

		struct __declspec(dllexport) key_mainline
		{
		public:
			std::vector<object_ref> object_refs;
			unsigned int id = 0;
		};

		struct __declspec(dllexport) timeline
		{
		public:
			std::vector<key_mainline> keys;
			unsigned int id=0;
			unsigned int length=0;
		};

		struct __declspec(dllexport) _mainline
		{
		public:
			std::vector<key_mainline> keys;
		};

		struct __declspec(dllexport) animation
		{
		public:
			_mainline mainline;
			std::vector<timeline> timelines;
			std::wstring name;
			unsigned int id=0;
			unsigned int length=0;//frame
		};

		struct __declspec(dllexport) entity
		{
		public:
			std::wstring name;
			std::vector<animation> animations;
			unsigned int id=0;
		};

		struct __declspec(dllexport) scml
		{
			std::vector<folder> folder;
			std::vector<entity> entity;
		};

		__declspec(dllexport) scml ReadSCML(std::wstring path);
	}
}