#pragma once

#include <filesystem>

#include "../sk_common.hxx"
#include "sk_string.hxx"
#include "sk_web_utils.hxx"

#if SK_OS == windows
	#include <windows.h>
#elif SK_OS == macos || SK_OS == ios
	#include <sys/stat.h>
#endif


BEGIN_SK_NAMESPACE


struct SK_File_Time {
	std::chrono::system_clock::time_point atime_tp;
	std::chrono::system_clock::time_point mtime_tp;
	std::chrono::system_clock::time_point ctime_tp;

	uint64_t atime = -1;
	uint64_t mtime = -1;
	uint64_t ctime = -1;
};

struct SK_File_Info {
	uint64_t volumeSerialNumber;
	SK_String mode;
	int numberOfLinks;

	uint64_t ino;

	char* atime;
	char* mtime;
	char* ctime;
};

class SK_File : public SK_String {
public:
	SK_String error;
	SK_String mimeType;
	nlohmann::json fileInfo;

	

	bool loadFromDisk(const SK_String& path, bool includeFileInfo = false){
		if (!exists(path)) return false;

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

			if (includeFileInfo) fileInfo = getFileInfo(path);

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



	/**********************/
	#if SK_OS == windows
		static std::chrono::system_clock::time_point fileTimeToChrono(const FILETIME& ft) {
			ULARGE_INTEGER ull;
			ull.LowPart = ft.dwLowDateTime;
			ull.HighPart = ft.dwHighDateTime;
			return std::chrono::system_clock::time_point(
				std::chrono::duration_cast<std::chrono::system_clock::duration>(
					std::chrono::nanoseconds(ull.QuadPart * 100)
				)
			);
		}

	#elif SK_OS == macos || SK_OS == ios
		std::chrono::system_clock::time_point timespecToChrono(const struct timespec& ts) {
			return std::chrono::system_clock::time_point(
				std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec)
			);
		}
	#endif

	static uint64_t timePointToUint64(const std::chrono::system_clock::time_point& timePoint) {
		auto durationSinceEpoch = timePoint.time_since_epoch();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(durationSinceEpoch).count();
	}


	static void getFileTimestamps(const SK_String& path, SK_File_Time* _fileTime) {
		if (!exists(path)) return;

		SK_File_Time fileTime;

		#if SK_OS == windows 
			HANDLE file = CreateFileA(path.data.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file == INVALID_HANDLE_VALUE) {
				__debugbreak();
			} else {
			

				FILETIME creationTime, lastAccessTime, lastWriteTime;
				if (GetFileTime(file, &creationTime, &lastAccessTime, &lastWriteTime)) {
					fileTime.ctime_tp = fileTimeToChrono(creationTime);
					fileTime.atime_tp = fileTimeToChrono(lastAccessTime);
					fileTime.mtime_tp = fileTimeToChrono(lastWriteTime);

					fileTime.ctime = timePointToUint64(fileTime.ctime_tp);
					fileTime.atime = timePointToUint64(fileTime.atime_tp);
					fileTime.mtime = timePointToUint64(fileTime.mtime_tp);
				} else {
					__debugbreak();
				}
			}

			CloseHandle(file);
		#elif SK_OS == macos || SK_OS == ios
			struct stat fileStat;
			if (stat(filePath.c_str(), &fileStat) == 0) {
				auto timespecToChrono = [](const struct timespec& ts) {
					return std::chrono::system_clock::time_point(
						std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec));
					};

				auto creation = timespecToChrono(fileStat.st_ctimespec);
				auto access = timespecToChrono(fileStat.st_atimespec);
				auto modification = timespecToChrono(fileStat.st_mtimespec);

				std::cout << "Creation Time: " << std::chrono::system_clock::to_time_t(creation) << std::endl;
				std::cout << "Last Access Time: " << std::chrono::system_clock::to_time_t(access) << std::endl;
				std::cout << "Last Modification Time: " << std::chrono::system_clock::to_time_t(modification) << std::endl;
			}
			else {
				std::cerr << "Error: Cannot get file stats." << std::endl;
			}
		#endif

		_fileTime->atime = fileTime.atime;
		_fileTime->ctime = fileTime.ctime;
		_fileTime->mtime = fileTime.mtime;
	}

    static bool exists(const SK_String& path){
		try {
			return std::filesystem::exists(std::filesystem::path(path));
		}
		catch (const std::filesystem::filesystem_error& e) {
			__debugbreak();
		}

		return false;
    }

    static bool isDirectory(const SK_String& path) {
		if (!exists(path)) return false;

        return std::filesystem::is_directory(std::filesystem::path(path));
    }

    static nlohmann::json getFileInfo(const SK_String& path) {
		nlohmann::json statInfo{ {"error", "ENOENT"} };

		if (!exists(path)) return statInfo;


		SK_File_Info fileInfo;

		SK_File_Time fileTime;
		getFileTimestamps(path, &fileTime);


		if (isDirectory(path)) {
			//uint64_t size = static_cast<uint64_t>(std::filesystem::file_size(std::filesystem::path(path)));
			statInfo = nlohmann::json {
				{"type"			, "dir"},
				{"dev"			, "" },
				{"mode"			, -1 },
				{"nlink"		, 0  },
				{"uid"			, 0  },
				{"gid"			, 0  },
				{"rdev"			, 0  },
				{"blksize"		, -1 },
				{"ino"			, 0  },
				{"size"			, "" },//size },
				{"blocks"		, -1 },
				{"atimeMs"		, "" },//fileTime.atime },
				{"mtimeMs"		, "" },//fileTime.mtime },
				{"ctimeMs"		, "" },//fileTime.ctime },
				{"birthtimeMs"	, "" },//SK_String(fileTime.ctime) },
				{"atime"		, "" },//SK_DateTime::formatTime(fileTime.atime_tp, "%Y-%m-%dT%H:%M:%S.000Z") },
				{"mtime"		, "" },//SK_DateTime::formatTime(fileTime.mtime_tp, "%Y-%m-%dT%H:%M:%S.000Z") },
				{"ctime"		, "" },//SK_DateTime::formatTime(fileTime.ctime_tp, "%Y-%m-%dT%H:%M:%S.000Z") },
				{"birthtime"	, "" }//SK_DateTime::formatTime(fileTime.ctime_tp, "%Y-%m-%dT%H:%M:%S.000Z") }
			};
		} else {
#			if SK_OS == windows
				HANDLE hFile = CreateFile(path.data.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE) {
					//return error "ENOENT"
					return statInfo;
				}

				BY_HANDLE_FILE_INFORMATION _fileInfo;
				if (!GetFileInformationByHandle(hFile, &_fileInfo)) {
					//return error "ENOENT"
					CloseHandle(hFile);
					return statInfo;
				}

				CloseHandle(hFile);

				fileInfo.volumeSerialNumber = (uint64_t)_fileInfo.dwVolumeSerialNumber;
				fileInfo.numberOfLinks = (uint64_t)_fileInfo.nNumberOfLinks;
				fileInfo.ino = ((uint64_t)_fileInfo.nFileIndexHigh << 32) | _fileInfo.nFileIndexLow;

			#elif SK_OS == macos || SK_OS == ios

				struct stat fileStat;

				// Get file statistics
				if (stat(filePath.c_str(), &fileStat) != 0) {
					std::cerr << "Failed to get file stats for " << filePath << std::endl;
					return fileInfo;
				}

				// Print file size
				//std::cout << "File size: " << fileStat.st_size << " bytes" << std::endl;

				// Print file permissions
				char* permissions;
				/*permissions << ((fileStat.st_mode & S_IRUSR) ? "r" : "-")
							  << ((fileStat.st_mode & S_IWUSR) ? "w" : "-")
							  << ((fileStat.st_mode & S_IXUSR) ? "x" : "-")
							  << ((fileStat.st_mode & S_IRGRP) ? "r" : "-")
							  << ((fileStat.st_mode & S_IWGRP) ? "w" : "-")
							  << ((fileStat.st_mode & S_IXGRP) ? "x" : "-")
							  << ((fileStat.st_mode & S_IROTH) ? "r" : "-")
							  << ((fileStat.st_mode & S_IWOTH) ? "w" : "-")
							  << ((fileStat.st_mode & S_IXOTH) ? "x" : "-")
							  << std::endl;
				*/

				fileInfo.atime = std::ctime(&fileStat.st_atime);
				fileInfo.mtime = std::ctime(&fileStat.st_mtime);
				fileInfo.ctime = std::ctime(&fileStat.st_ctime);
			#endif



			statInfo = nlohmann::json{
				{"type"			, "file"},
				{"dev"			, fileInfo.volumeSerialNumber },
				{"mode"			, -1 },
				{"nlink"		, fileInfo.numberOfLinks },
				{"uid"			, 0  },
				{"gid"			, 0  },
				{"rdev"			, 0  },
				{"blksize"		, -1 },
				{"ino"			, fileInfo.ino },
				{"size"			, "" },//std::filesystem::file_size(std::filesystem::path(path)) },
				{"blocks"		, -1 },
				{"atimeMs"		, "" },//fileTime.atime },
				{"mtimeMs"		, "" },//fileTime.mtime },
				{"ctimeMs"		, "" },//fileTime.ctime },
				{"birthtimeMs"	, "" },//SK_String(fileTime.ctime) },
				{"atime"		, "" },//SK_DateTime::formatTime(fileTime.atime_tp, "%Y-%m-%dT%H:%M:%S.000Z") },
				{"mtime"		, "" },//SK_DateTime::formatTime(fileTime.mtime_tp, "%Y-%m-%dT%H:%M:%S.000Z") },
				{"ctime"		, "" },//SK_DateTime::formatTime(fileTime.ctime_tp, "%Y-%m-%dT%H:%M:%S.000Z") },
				{"birthtime"	, "" },//SK_DateTime::formatTime(fileTime.ctime_tp, "%Y-%m-%dT%H:%M:%S.000Z") }
			};
		}

		return statInfo;
    }
};


END_SK_NAMESPACE