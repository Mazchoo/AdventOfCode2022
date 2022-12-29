#pragma once
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


template <typename T>
struct BranchNode {
	BranchNode() {}
	BranchNode(BranchNode* parent) : mParent(parent) {}
	BranchNode(BranchNode* parent, T value) : mParent(parent), mValue(value) { hasValue = true; }

	inline void appendLeaf(T val) { mChildren.emplace_back(BranchNode(this, val)); }
	inline void appendBranch() { mChildren.emplace_back(BranchNode(this)); }
	inline void setValue(T val) { mValue = val; hasValue = true; }
	int length() { return mChildren.size();  }

	explicit operator bool() const { return hasValue;  }
	BranchNode& operator [] (int index) {
		auto childrenFront = mChildren.begin();
		std::advance(childrenFront, index);
		return *childrenFront;
	}

	bool hasValue = false;
	BranchNode* mParent = nullptr;
	T mValue = NULL;
	std::list<BranchNode> mChildren = {};
};
