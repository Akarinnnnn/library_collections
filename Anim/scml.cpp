#include "stdafx.h"
#include "scml.h"
#include <pugiconfig.hpp>
#include <pugixml.hpp>

using namespace Anim::SCML;
using namespace std;
scml Anim::SCML::ReadSCML(std::wstring path)
{
	using namespace pugi;
	xml_document scmldoc;
	scmldoc.load_file(path.c_str());
	auto root = scmldoc.child(L"spriter_data");
	vector<folder> folders;
	vector<entity> entities;

	for (auto _folder : root.children(L"folder"))
	{
		vector<file> files;
		wstring foldername = _folder.attribute(L"name").as_string();
		unsigned int folderid = _folder.attribute(L"id").as_uint();
		for (auto _file : _folder.children(L"file"))
		{
			file tmp =
			{
				_file.attribute(L"id").as_uint(),
				_file.attribute(L"width").as_uint(),
				_file.attribute(L"height").as_uint(),
				_file.attribute(L"pivot_x").as_float(),
				_file.attribute(L"pivot_y").as_float(),
				_file.attribute(L"name").as_string()
			};
			files.push_back(tmp);
		}
		folders.push_back(folder() = { files,foldername,folderid });
	}
	for (auto _entity : root.children(L"entity"))
	{
		vector<animation> animations;
		for (auto _anim : _entity.children(L"animation"))
		{
			vector<timeline> timelines;
			_mainline mainline;
			{
				vector<key_mainline> keys;
				for (auto _k : _anim.child(L"mainline").children(L"key"))
				{
					std::vector<object_ref> object_refs;
					for (auto objref : _k.children())
					{
						object_refs.push_back(object_ref() =
							{
								objref.attribute(L"id").as_uint(),
								objref.attribute(L"name").as_string(),
								objref.attribute(L"folder").as_uint(),
								objref.attribute(L"file").as_uint(),
								objref.attribute(L"z_index").as_uint(),
								objref.attribute(L"abs_pivot_x").as_float(),
								objref.attribute(L"abs_pivot_y").as_float(),
								objref.attribute(L"abs_angle").as_float(),
								objref.attribute(L"abs_scale_x").as_float(),
								objref.attribute(L"abs_scale_y").as_float()
							}
						);
					}
					keys.push_back(key_mainline() =
					{
						object_refs,
						_k.attribute(L"id").as_uint()
					});
				}
			}
			for (auto _tl : _anim.children(L"timeline"))
			{
				vector<key> keys;
				for (auto _k : _tl.children(L"key"))
				{
					std::vector<object> objects;
					key temp;
					for (auto obj : _k.children())
					{
						objects.push_back(object() =
							{
								obj.attribute(L"id").as_uint(),
								obj.attribute(L"folder").as_uint(),
								obj.attribute(L"file").as_uint(),
								obj.attribute(L"abs_pivot_x").as_float(0.0f),
								obj.attribute(L"abs_pivot_y").as_float(0.0f),
								obj.attribute(L"abs_scale_x").as_float(0.0f),
								obj.attribute(L"abs_scale_y").as_float(0.0f)
							}
						);
					}
					temp.objects = objects;
					temp.id = _k.attribute(L"id").as_uint();
					keys.push_back(temp);
				}

			}
			animations.push_back(animation() =
				{
					mainline,
					timelines,
					_anim.attribute(L"name").as_string(),
					_anim.attribute(L"id").as_uint(),
					_anim.attribute(L"length").as_uint()
				});
		}
		entities.push_back(entity() = 
			{ 
				_entity.attribute(L"name").as_string(),
				animations,
				_entity.attribute(L"id").as_uint() 
			});
	}
	return scml() = { folders,entities };
}