#pragma once
#define EXPORTS __declspec(dllexport)
#include <algorithm>
#include "Util.h"
namespace Anim
{
	void SCML2Anim(SCML::scml source);
}