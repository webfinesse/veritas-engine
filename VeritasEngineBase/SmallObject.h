#pragma once

#include <memory>
#include <vector>
#include "Singleton.h"

// based on Loki's Small Object Allocator

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 4096
#endif

#ifndef MAX_SMALL_OBJECT_SIZE
#define MAX_SMALL_OBJECT_SIZE 64
#endif

namespace VeritasEngine
{
	class SmallObjectAllocator
	{
	public:
		SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize);
		~SmallObjectAllocator();

		void* Allocate(std::size_t numBytes);
		void Deallocate(void* p, std::size_t size);

	private:
		SmallObjectAllocator(const SmallObjectAllocator&) = delete;
		SmallObjectAllocator& operator=(const SmallObjectAllocator&) = delete;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

	template<std::size_t chunkSize = DEFAULT_CHUNK_SIZE, std::size_t maxSmallObjectSize = MAX_SMALL_OBJECT_SIZE>
	class SmallObject
	{
	public:
		static void* operator new(std::size_t size)
		{
			return InternalObjAllocator::Instance().Allocate(size);
		}

		static void operator delete(void* p, std::size_t size)
		{
			InternalObjAllocator::Instance().Deallocate(p, size);
		}

		virtual ~SmallObject() = default;

	private:
		struct InternalObjAllocator : SmallObjectAllocator, Singleton<InternalObjAllocator>
		{
			friend class Singleton<InternalObjAllocator>;

			InternalObjAllocator()
				: SmallObjectAllocator(chunkSize, maxSmallObjectSize)
			{

			}
		};
	};

	template<std::size_t chunkSize = DEFAULT_CHUNK_SIZE, std::size_t maxSmallObjectSize = MAX_SMALL_OBJECT_SIZE>
	class SmallPODObject
	{
	public:
		static void* operator new(std::size_t size)
		{
			return InternalObjAllocator::Instance().Allocate(size);
		}

		static void operator delete(void* p, std::size_t size)
		{
			InternalObjAllocator::Instance().Deallocate(p, size);
		}

	private:
		struct InternalObjAllocator : SmallObjectAllocator, Singleton<InternalObjAllocator>
		{
			friend class Singleton<InternalObjAllocator>;

			InternalObjAllocator()
				: SmallObjectAllocator(chunkSize, maxSmallObjectSize)
			{

			}
		};
	};
}
