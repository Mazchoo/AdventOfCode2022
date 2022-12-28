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
	BranchNode(BranchNode* parent, T value) : mParent(parent), mValue(value) {}

	inline void appendLeaf(T val) { mChildren.emplace_back(BranchNode(this, val)); }
	inline void appendBranch() { mChildren.emplace_back(BranchNode(this)); }
	explicit operator bool() const { return mChildren.size() > 0;  }

	BranchNode* mParent = nullptr;
	T mValue = NULL;
	std::list<BranchNode> mChildren = {};
};
