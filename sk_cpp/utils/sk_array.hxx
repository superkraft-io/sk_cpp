#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <unordered_map>
/*
class SK_Array {
public:
    std::unordered<std::string, std::any> list;
    
    void push(const std::string& key, std::any value){
        list[key] = value;
    }
}

#elif defined(__APPLE__)

 
class SK_Array {
public:
    NSMutableDictionary list;
    
    void push(const std::string& key, std::any value){
        [list setObject:value forKey:key]
    }
}
#endif
