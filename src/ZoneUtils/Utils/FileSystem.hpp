#pragma once

#undef CreateDirectory

namespace ZoneTool
{
	class FileSystem
	{
	private:
		static std::string m_fastfile;
		static bool preferLocal;
		static bool forceExternalAssets;

	public:
		FileSystem()
		{
		}

		~FileSystem()
		{
		}

		static void ForceExternalAssets(bool force);
		static bool IsExternalFile(FILE* fp);
		static std::string GetDestFolderForAsset(const std::string& name);
		static std::string GetRelativeFolderForFile(FILE* fp);
		static std::string GetFullPathForFile(FILE* fp);
		static void CreateDirectory(const std::string& name);
		static std::string GetFilePath(const std::string& name);
		static void PreferLocalOverExternal(bool state);
		static bool FileExists(const std::string& name);
		static void SetFastFile(const std::string& ff);
		static std::string GetFastFile();
		static FILE* FileOpen(const std::string& name, const std::string& mode);
		static std::size_t FileSize(FILE* fp);
		static void FileClose(FILE* fp);
		static std::vector<std::uint8_t> ReadBytes(FILE* fp, std::size_t size);
		static std::shared_ptr<FileReader> ToReader(FILE* fp);
	};
}
