
#include <vector>

template <typename T>
struct BufferedList {
	inline T* getPointer() { return mData.data(); }
	inline void preallocate(size_t length) { mData.resize(length); }
	inline void reset() { mLength = 0; };
	inline void append(T val) { mData[mLength++] = val; }

	size_t mLength = 0;
	std::vector<T> mData;
};
