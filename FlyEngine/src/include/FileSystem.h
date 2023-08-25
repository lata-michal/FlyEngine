#pragma once

#include "pch.h"

namespace feng {

	class FileSystem
	{
	private:
		std::string m_ExecutableDirPath;
	public:
		FileSystem()
			: m_ExecutableDirPath(std::filesystem::path(std::filesystem::current_path()).string()) { }

		const std::string& GetExecutableDirPath() const { return m_ExecutableDirPath; }

		//to be expanded
	};

}