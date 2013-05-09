#ifdef _WIN32
#define constexpr const
#define noexcept

namespace std { //initializer_list hack
	//dont actually use initialzer lists but we can leave the method there with this
	template<typename T>
	struct initializer_list {
		T* begin() const {
			return nullptr;
		}
		T* end() const {
			return nullptr;
		}
	};
} //end std hack
#endif
