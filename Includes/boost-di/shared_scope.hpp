#ifndef H_SHARED_SCOPE
#define H_SHARED_SCOPE

#include <../Includes/boost-di/di.hpp>

namespace di = boost::di;

class shared_scope {
public:
	template <class, class T>
	class scope {
	public:
		template <class T_>
		using is_referable = typename di::wrappers::shared<shared_scope, T>::template is_referable<T_>;

		template <class, class, class TProvider>
		static auto try_create(const TProvider& provider)
			-> decltype(di::wrappers::shared<shared_scope, T>{std::shared_ptr<T>{provider.get()}});

		template <class, class, class TProvider>
		auto create(const TProvider& provider) {
			if (!object_) {
				object_ = std::shared_ptr<T>{ provider.get() };
			}
			return di::wrappers::shared<shared_scope, T>{object_};
		}

	private:
		std::shared_ptr<T> object_;
	};
};

static constexpr shared_scope shared{};

#endif