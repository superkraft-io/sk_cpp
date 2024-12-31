#pragma once

#include <filesystem>

#include "../sk_common.hxx"
#include "sk_string.hxx"
#include "sk_web_utils.hxx"

BEGIN_SK_NAMESPACE

class SK_File : public SK_String {
public:
	SK_String error;
	SK_String mimeType;

	bool loadFromDisk(const SK_String& path){
		FILE* file = fopen(path.replaceAll("\\", "/").c_str(), "rb");
		if (file) {
			fseek(file, 0, SEEK_END);
			long dataSize = ftell(file);
			char* buffer = (char*)malloc(dataSize + 1);
			fseek(file, 0, SEEK_SET);
			fread(buffer, 1, dataSize, file);

			data.resize(dataSize);
			std::memcpy(data.data(), buffer, dataSize);

			free(buffer);

			fclose(file);

			mimeType = SK_Web_Utils::mime.fromFilename(path);

			return true;
		}

		//__debugbreak(); //Could not load file from disk. Maybe the path is wrong?

		return false;
	}

	bool loadFromBDFS(const SK_String& path){
		//Load from binsry data
	}

	bool loadFromVFS(const SK_String& path) {
		//Load from virtual filesystem
	}


	bool saveToDisk(const SK_String& path) {
		//Save to disk
	}

	bool saveToVFS(const SK_String& path) {
		//Save to virtual fileaystem
	}
};


END_SK_NAMESPACE