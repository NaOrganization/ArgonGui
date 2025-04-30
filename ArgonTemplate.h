#pragma once
#include <functional>

template<typename T, typename = void>
struct ArIsEnumFlag : std::false_type {};

template<typename T>
struct ArIsEnumFlag<T, std::enable_if_t<
	std::is_enum<T>::value&&
	std::is_same<typename std::underlying_type<T>::type, uint32_t>::value&&
	std::is_convertible<decltype(T::None), T>::value
	>> : std::true_type {};

template<typename Enum>
typename std::enable_if<ArIsEnumFlag<Enum>::value, Enum>::type
operator|(Enum lhs, Enum rhs)
{
	using Underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
}

template<typename Enum>
typename std::enable_if<ArIsEnumFlag<Enum>::value, Enum>::type
operator&(Enum lhs, Enum rhs)
{
	using Underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
}

template<typename Enum>
typename std::enable_if<ArIsEnumFlag<Enum>::value, bool>::type
ArHasFlag(Enum value, Enum flag)
{
	using Underlying = typename std::underlying_type<Enum>::type;
	return (static_cast<Underlying>(value) & static_cast<Underlying>(flag)) != 0;
}

template<typename Enum, typename... EnumFlags>
typename std::enable_if<ArIsEnumFlag<Enum>::value && (ArIsEnumFlag<EnumFlags>::value && ...), bool>::type
ArHasAnyFlag(Enum value, EnumFlags... flags)
{
	return (ArHasFlag(value, flags) || ...);
}

template <typename T, size_t BlockSize = 2042, typename Allocator = std::allocator<T>>
class ArChunkedVector {
private:
	Allocator allocator = {};
	std::vector<T*> chunks = {};
	T* currentChunk = nullptr;
	size_t currentIndex = 0;
	size_t elementSize = 0;
	T* contiguousData = nullptr;

	void allocate_block() {
		T* newBlock = allocator.allocate(BlockSize);
		chunks.push_back(newBlock);
		currentChunk = newBlock;
		currentIndex = 0;
	}

	void EnsureCapacityForOne() {
		if (currentChunk == nullptr || currentIndex >= BlockSize)
			allocate_block();
	}

public:
	ArChunkedVector() : currentChunk(nullptr), currentIndex(0), elementSize(0), contiguousData(nullptr) {}

	~ArChunkedVector() {
		clear();
		for (T* block : chunks)
			allocator.deallocate(block, BlockSize);
		chunks.clear();
		free_data();
	}

	ArChunkedVector(const ArChunkedVector&) = delete;
	ArChunkedVector& operator=(const ArChunkedVector&) = delete;

	void push_back(const T& value) {
		EnsureCapacityForOne();
		new (currentChunk + currentIndex) T(value);
		++currentIndex;
		++elementSize;
	}

	void push_back(T&& value) {
		EnsureCapacityForOne();
		new (currentChunk + currentIndex) T(std::move(value));
		++currentIndex;
		++elementSize;
	}

	template<typename... Args>
	void emplace_back(Args&&... args) {
		EnsureCapacityForOne();
		new (currentChunk + currentIndex) T(std::forward<Args>(args)...);
		++currentIndex;
		++elementSize;
	}

	T* data() {
		if (contiguousData)
			return contiguousData;
		if (elementSize == 0)
			return nullptr;
		contiguousData = new T[elementSize];
		size_t copied = 0;
		for (size_t b = 0; b < chunks.size(); ++b) {
			size_t count = (b == chunks.size() - 1) ? currentIndex : BlockSize;
			std::copy(chunks[b], chunks[b] + count, contiguousData + copied);
			copied += count;
		}
		return contiguousData;
	}

	void free_data() {
		if (contiguousData) {
			delete[] contiguousData;
			contiguousData = nullptr;
		}
	}

	T& operator[](size_t index) {
		if (index >= elementSize)
			throw std::out_of_range("Index out of range");
		size_t blockIndex = index / BlockSize;
		size_t offset = index % BlockSize;
		return chunks[blockIndex][offset];
	}

	const T& operator[](size_t index) const {
		if (index >= elementSize)
			throw std::out_of_range("Index out of range");
		size_t blockIndex = index / BlockSize;
		size_t offset = index % BlockSize;
		return chunks[blockIndex][offset];
	}

	void clear() {
		for (size_t b = 0; b < chunks.size(); ++b) {
			size_t count = (b == chunks.size() - 1) ? currentIndex : BlockSize;
			for (size_t j = 0; j < count; ++j)
				(chunks[b] + j)->~T();
		}
		elementSize = 0;
		currentChunk = (chunks.empty() ? nullptr : chunks[0]);
		currentIndex = 0;
		free_data();
	}

	size_t size() const { return elementSize; }

	size_t capacity() const { return chunks.size() * BlockSize; }

	void reserve(size_t newCapacity) {
		size_t currentCapacity = capacity();
		while (currentCapacity < newCapacity) {
			allocate_block();
			currentCapacity += BlockSize;
		}
	}

	void resize(size_t newSize) {
		if (newSize < elementSize) {
			size_t newFullBlocks = newSize / BlockSize;
			size_t newLastCount = newSize % BlockSize;
			if (!chunks.empty() && newSize > 0) {
				if (newFullBlocks < chunks.size()) {
					size_t destroyCount = (newFullBlocks == chunks.size() - 1) ? (currentIndex - newLastCount) : BlockSize;
					T* block = chunks[newFullBlocks];
					for (size_t j = newLastCount; j < ((newFullBlocks == chunks.size() - 1) ? currentIndex : BlockSize); ++j)
						(block + j)->~T();
				}
			}
			elementSize = newSize;
			if (newSize == 0) {
				currentChunk = nullptr;
				currentIndex = 0;
			}
			else {
				size_t fullBlocks = newSize / BlockSize;
				size_t last = newSize % BlockSize;
				if (last == 0) {
					currentChunk = chunks[fullBlocks - 1];
					currentIndex = BlockSize;
				}
				else {
					currentChunk = chunks[fullBlocks];
					currentIndex = last;
				}
			}
		}
		else if (newSize > elementSize) {
			while (elementSize < newSize)
				push_back(T());
		}
	}
};

template<typename T>
class ArValueSubject
{
public:
	using Observer = std::function<void(const T&)>;
private:
	std::vector<Observer> observers;
	T value;
public:
	ArValueSubject() : value() {}
	ArValueSubject(const T& initialValue) : value(initialValue) {}
	void SetValue(const T& newValue)
	{
		value = newValue;
		for (const auto& observer : observers)
			observer(value);
	}
	const T& GetValue() const { return value; }
	void AddObserver(Observer observer) { observer(value); observers.push_back(observer); }

	T& operator=(const T& newValue) { SetValue(newValue); return value; }

	T& operator*() { return value; }

	T* operator->() { return &value; }

	const T& operator*() const { return value; }

	const T* operator->() const { return &value; }

	bool operator==(const T& other) const { return value == other; }

	bool operator!=(const T& other) const { return value != other; }
};
