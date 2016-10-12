#ifndef H_SINGLETON
#define H_SINGLETON

namespace VeritasEngine
{
	template<typename T>
	class Singleton
	{
	public:
		static T& Instance()
		{
			static T instance;

			return instance;
		}

	protected:
		Singleton()
		{

		};
		
		virtual ~Singleton()
		{

		};

	private:
		Singleton(Singleton& rhs) = delete;
		Singleton& operator=(Singleton& rhs) = delete;

		Singleton(Singleton&& rhs) = delete;
		Singleton& operator=(Singleton&& rhs) = delete;
	};
}

#endif