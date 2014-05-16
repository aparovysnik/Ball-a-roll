#pragma once

template <class T >
class Singleton
{
	public:
		static void Create() { if (instance == NULL) { instance = new T(); } }
		static void Destroy() { delete instance; }
		static T & Instance() {
				return *instance;
			}
	protected:
		Singleton(void) { };
		virtual ~Singleton(void) { }
		static T * instance;
};
template < class T > T * Singleton <T >::instance = 0;

