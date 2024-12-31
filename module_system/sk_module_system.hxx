#pragma once

#include "../core/sk_common.hxx"

BEGIN_SK_NAMESPACE

class SK_Module_System {
public:
	void performOperation(const SK_String& module, const SK_String& operation, const SK_String& data);
};

END_SK_NAMESPACE
