#pragma once
#include <exception>
#include "export.h"
namespace KleiAnim
{
	namespace Exception
	{
		class EXPORT_API invalid_file : public std::exception
		{
		public:
			invalid_file();
			invalid_file(const char* message);
			invalid_file(const char* message, unsigned int cc4, unsigned short version);
			~invalid_file();

			unsigned int get_cc4();
			unsigned int get_version();

		private:
			unsigned int cc4;
			unsigned short version;
		};
	}
}