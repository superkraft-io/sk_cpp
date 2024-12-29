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
		/**** Used only in DEBUG mode when accessing filesystem directly ****/
		
		//When running in RELEASE mode, a pre-script will bundle all the files in "superkraft" and "SK_Project" and SK will access those files from memory instead.

		add("superkraft", SK_Path_Utils::pathBackwardsUntilNeighbour("superkraft") + "/superkraft");
		add("sk_project", SK_Path_Utils::pathBackwardsUntilNeighbour("sk_project") + "/sk_project");
		
		//System paths
		//paths["home"] = SK_Path_Utils::pathBackwardsUntilNeighbour("SK_Project").value_or(std::filesystem::path{});

		/********************************************************************/
	}
};


END_SK_NAMESPACE