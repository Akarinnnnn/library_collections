/***************************
* Encoding: GB2312
***************************/
// pch.h: ����Ԥ�����ͷ�ļ���
// �·��г����ļ�������һ�Σ�����˽������ɵ��������ܡ�
// �⻹��Ӱ�� IntelliSense ���ܣ�����������ɺ�������������ܡ�
// ���ǣ�����˴��г����ļ��е��κ�һ��������֮���и��£�����ȫ�����������±��롣
// �����ڴ˴����ҪƵ�����µ��ļ����⽫ʹ������������Ч��

#ifndef PCH_H
#define PCH_H

// ���Ҫ�ڴ˴�Ԥ����ı�ͷ
#include "framework.h"
#include "KleiAnim/common/anim_common.hpp"
#include "KleiAnim/common/exceptions.hpp"

#ifdef _WIN32
namespace win_spec
{
	HMODULE hmoudle;
}
#endif // _WIN32


#endif //PCH_H
