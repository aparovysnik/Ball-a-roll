#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
	m_Resources.clear();
}

void ResourceManager::FreeResource(const string& sourcePath)
{
	if (m_Resources[sourcePath] != 0)
	{
		std::cout << "Removed resource!" << std::endl;
		m_Resources[sourcePath]->Unload();
		m_Resources.erase(sourcePath);
	}
}
