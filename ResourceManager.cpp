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
		m_Resources[sourcePath]->Unload();
	}
}
