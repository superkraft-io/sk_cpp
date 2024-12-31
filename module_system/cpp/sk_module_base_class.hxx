#pragma once

#include "map"
#include "../../core/sk_common.hxx"


using SK_Module_Base_Callback = std::function<void(SK_String& data)>;

class SK_Module_Base_Class {
public:
    SK_Module_Base_Class::SK_Module_Base_Class();
    
    std::map<SK_String, SK_Module_Base_Callback> operations;
};
