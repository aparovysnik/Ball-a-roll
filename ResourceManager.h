#pragma once
#include <map>
#include <string>
#include "Singleton.h"
#include "Resource.h"

#include "TextureResource.h"
#include "MusicResource.h"
#include "SoundEffectResource.h"
#include "LevelResource.h"
#include "ShaderResource.h"

using namespace std;

// Resource manager
class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

	map< string, Resource * > m_Resources;
public:
	template< class T >
	T * GetResource(const string & sourcePath)
	{
		if (m_Resources[sourcePath] != 0)
		{
			return dynamic_cast< T * >(m_Resources[sourcePath]);
		}

		T * newT = new T();
		
		if (!newT->Load(sourcePath))
			return NULL;
		m_Resources[sourcePath] = newT;
		return newT;
	}

	template < class T >
	void StoreResource(string id, T* res)
	{
		m_Resources[id] = res;
	}

	void FreeResource(const string & sourcePath);

protected:
	ResourceManager(void);
	 ~ResourceManager(void);
};

