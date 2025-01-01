#pragma once

#include <map>
#include <optional>
#include <string>
#include <filesystem>

#include "../sk_common.hxx"
#include "../../libs/general/choc/text/choc_StringUtilities.h"

#include "../utils/sk_string.hxx"

BEGIN_SK_NAMESPACE

static class SK_Path_Utils {
public:
	static inline std::map<std::string, std::string> paths;

	static std::string pathBackwardsUntilNeighbour(const std::string& neighbourName){
		std::filesystem::path currentPath(__FILE__);

		bool stop = false;
		while (!stop) {
			currentPath = currentPath.parent_path();

			if (currentPath == currentPath.root_path()) {
				return "";
			}

			for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
				if (entry.is_directory() && choc::text::toLowerCase(entry.path().filename().string()) == choc::text::toLowerCase(neighbourName)) {
					stop = true;
					break;
				}
			}
		}

		return currentPath.string();
	}

	static std::string getProjectRoot() {
		std::filesystem::path mainPath(__FILE__);

		std::string path = mainPath.string();
		std::filesystem::path indexRelativePath = mainPath.parent_path() / "Resources" / "web" / "index.html";

		return path;
	}

	static void add(const SK_String& id, const SK_String& path) {
		paths[id] = path.replaceAll("\\", "/");
	}


	static void init() {
		#ifdef SK_MODE_DEBUG
			//In DEBUG mode the projectRoot will be set to the root folder of your project.
			SK_String projectRoot = SK_String(SK_Path_Utils::pathBackwardsUntilNeighbour("superkraft")).replaceAll("\\", "/");
		#else
			//In RELEASE mode the projectRoot will be set to the virtual file system called Binary Data File Systtem
			//When running in RELEASE mode, a pre-script will bundle all the files in "superkraft" and "SK_Project" and SK will access those files from memory instead.
			SK_String projectRoot = "sk_bdfs:";
		#endif

		SK_String superkraftRoot = projectRoot + "/superkraft";

		add("superkraft", superkraftRoot);
		add("global_js_core", superkraftRoot + "/sk_cpp/web/global_js_core");
		add("module_system", superkraftRoot + "/sk_cpp/module_system/web");
		add("soft_backend", superkraftRoot + "/sk_cpp/web/soft_backend");
		add("sk_project", projectRoot + "/sk_project");
		add("config", paths["sk_project"] + "/config.json");
		
		//The paths below will be OS specific
		add("home", "");
		add("temp", "");
	}
};


END_SK_NAMESPACE