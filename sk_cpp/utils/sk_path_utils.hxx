#pragma once

#include <map>
#include <optional>
#include <string>
#include <filesystem>

#include "../sk_common.hxx"
#include "../../libs/general/choc/text/choc_StringUtilities.h"

BEGIN_SK_NAMESPACE

class SK_Path_Utils {
public:
	static std::map<std::string, std::filesystem::path> paths;

	static std::optional<std::filesystem::path> pathBackwardsUntilNeighbour(const std::string& neighbourName) {
		std::filesystem::path currentPath(__FILE__);

		bool stop = false;
		while (!stop) {
			currentPath = currentPath.parent_path();

			if (currentPath == currentPath.root_path()) {
				return std::nullopt;
			}

			for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
				if (entry.is_directory() && choc::text::toLowerCase(entry.path().filename().string()) == choc::text::toLowerCase(neighbourName)) {
					stop = true;
					break;
				}
			}
		}

		return static_cast<std::filesystem::path>(currentPath);
	}

	static std::string getProjectRoot() {
		std::filesystem::path mainPath(__FILE__);

		std::string path = mainPath.string();
		std::filesystem::path indexRelativePath = mainPath.parent_path() / "Resources" / "web" / "index.html";

		return path;
	}



	SK_Path_Utils::SK_Path_Utils() {
		/**** Used only in DEBUG mode when accessing filesystem directly ****/
		
		//When running in RELEASE mode, a pre-script will bundle all the files in "superkraft" and "SK_Project" and SK will access those files from memory instead.

		paths["superkraft"] = SK_Path_Utils::pathBackwardsUntilNeighbour("superkraft").value_or(std::filesystem::path{});
		paths["sk_project"] = SK_Path_Utils::pathBackwardsUntilNeighbour("SK_Project").value_or(std::filesystem::path{});

		/********************************************************************/
	}
};

END_SK_NAMESPACE