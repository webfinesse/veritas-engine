#include "SmallObject.h"

#include <algorithm>
#include <cassert>

namespace VeritasEngine {
	class Chunk
	{
		friend class SmallObjectFixedAllocator;

	private:
		void Init(std::size_t blockSize, unsigned char blocks)
		{
			assert(blockSize > 0);
			assert(blocks > 0);
			// Overflow check
			assert((blockSize * blocks) / blockSize == blocks);

			m_data = new unsigned char[blockSize * blocks];
			Reset(blockSize, blocks);
		}

		void* Allocate(std::size_t blockSize)
		{
			if (!m_blocksAvailable)
			{
				return 0;
			}

			assert((m_firstAvailableBlock * blockSize) / blockSize == m_firstAvailableBlock);

			unsigned char* result = m_data + (m_firstAvailableBlock * blockSize);
			m_firstAvailableBlock = *result;
			--m_blocksAvailable;

			return result;
		}

		void Deallocate(void* p, std::size_t blockSize)
		{
			assert(p >= m_data);

			unsigned char* toRelease = static_cast<unsigned char*>(p);
			// Alignment check
			assert((toRelease - m_data) % blockSize == 0);

			*toRelease = m_firstAvailableBlock;
			m_firstAvailableBlock = static_cast<unsigned char>((toRelease - m_data) / blockSize);
			// Truncation check
			assert(m_firstAvailableBlock == (toRelease - m_data) / blockSize);

			++m_blocksAvailable;
		}

		void Reset(std::size_t blockSize, unsigned char blocks)
		{
			assert(blockSize > 0);
			assert(blocks > 0);
			// Overflow check
			assert((blockSize * blocks) / blockSize == blocks);

			m_firstAvailableBlock = 0;
			m_blocksAvailable = blocks;

			unsigned char* p = m_data;
			for (unsigned char i = 0; i != blocks; p += blockSize)
			{
				*p = ++i;
			}
		}

		void Release()
		{
			delete[] m_data;
		}

		unsigned char* m_data;
		unsigned char m_firstAvailableBlock;
		unsigned char m_blocksAvailable;
	};

	class SmallObjectFixedAllocator
	{
	public:
		explicit SmallObjectFixedAllocator(std::size_t blockSize = 0)
			: m_blockSize(blockSize), m_allocChunk(nullptr), m_deallocChunk(nullptr), m_prev(this), m_next(this)
		{
			assert(m_blockSize > 0);

			auto numblocks = MAX_SMALL_OBJECT_SIZE / blockSize;
			if (numblocks > UCHAR_MAX)
			{
				numblocks = UCHAR_MAX;
			}
			else if (numblocks == 0)
			{
				numblocks = 8 * blockSize;
			}

			m_numBlocks = static_cast<unsigned char>(numblocks);
			assert(m_numBlocks == numblocks);
		}

		SmallObjectFixedAllocator(const SmallObjectFixedAllocator& rhs)
			: m_blockSize(rhs.m_blockSize), m_numBlocks(rhs.m_numBlocks), m_chunks(rhs.m_chunks), m_prev(&rhs), m_next(rhs.m_next)
		{
			rhs.m_next->m_prev = this;
			rhs.m_next = this;

			m_allocChunk = rhs.m_allocChunk != nullptr ? &m_chunks.front() + (rhs.m_allocChunk - &rhs.m_chunks.front()) : nullptr;
			m_deallocChunk = rhs.m_deallocChunk != nullptr ? &m_chunks.front() + (rhs.m_deallocChunk - &rhs.m_chunks.front()) : nullptr;
		}

		SmallObjectFixedAllocator& operator=(const SmallObjectFixedAllocator& rhs)
		{
			SmallObjectFixedAllocator copy(rhs);
			copy.Swap(*this);
			return *this;
		}

		~SmallObjectFixedAllocator()
		{
			if (m_prev != this)
			{
				m_prev->m_next = m_next;
				m_next->m_prev = m_prev;
			}

			assert(m_prev == m_next);

			for (auto& chunk : m_chunks)
			{
				assert(chunk.m_blocksAvailable == m_numBlocks);
				chunk.Release();
			}
		}

		void Swap(SmallObjectFixedAllocator& rhs)
		{
			using namespace std;

			swap(m_blockSize, rhs.m_blockSize);
			swap(m_numBlocks, rhs.m_numBlocks);
			m_chunks.swap(rhs.m_chunks);
			swap(m_allocChunk, rhs.m_allocChunk);
			swap(m_deallocChunk, rhs.m_deallocChunk);
		}


		void* Allocate()
		{
			if (m_allocChunk == nullptr || m_allocChunk->m_blocksAvailable == 0)
			{
				auto iter = m_chunks.begin();
				for (;; ++iter)
				{
					if (iter == m_chunks.end())
					{
						m_chunks.reserve(m_chunks.size() + 1);
						m_chunks.emplace_back();
						m_chunks.back().Init(m_blockSize, m_numBlocks);
						m_allocChunk = &m_chunks.back();
						m_deallocChunk = &m_chunks.front();
						break;
					}

					if (iter->m_blocksAvailable > 0)
					{
						m_allocChunk = &*iter;
						break;
					}
				}
			}

			assert(m_allocChunk != nullptr);
			assert(m_allocChunk->m_blocksAvailable > 0);

			return m_allocChunk->Allocate(m_blockSize);
		}

		void Deallocate(void* p)
		{
			assert(!m_chunks.empty());
			assert(&m_chunks.front() <= m_deallocChunk);
			assert(&m_chunks.back() >= m_deallocChunk);

			m_deallocChunk = VicinityFind(p);
			assert(m_deallocChunk != nullptr);

			DoDeallocate(p);
		}

		std::size_t BlockSize() const
		{
			return m_blockSize;
		}

		bool operator<(std::size_t rhs) const
		{
			return m_blockSize < rhs;
		}

	private:

		void DoDeallocate(void* p)
		{
			m_deallocChunk->Deallocate(p, m_blockSize);

			if (m_deallocChunk->m_blocksAvailable == m_numBlocks)
			{
				auto& lastChunk = m_chunks.back();

				if (&lastChunk == m_deallocChunk)
				{
					if (m_chunks.size() > 1 && m_deallocChunk[-1].m_blocksAvailable == m_numBlocks)
					{
						lastChunk.Release();
						m_chunks.pop_back();
						m_allocChunk = &m_chunks.front();
						m_deallocChunk = &m_chunks.back();
					}

					return;
				}

				if (lastChunk.m_blocksAvailable == m_numBlocks)
				{
					lastChunk.Release();
					m_chunks.pop_back();
					m_allocChunk = m_deallocChunk;
				}
				else
				{
					std::swap(*m_deallocChunk, lastChunk);
					m_allocChunk = &m_chunks.back();
				}
			}
		}

		Chunk* VicinityFind(void* p)
		{
			assert(!m_chunks.empty());
			assert(m_deallocChunk);

			const std::size_t chunkLength = m_numBlocks * m_blockSize;

			auto lo = m_deallocChunk;
			auto hi = m_deallocChunk + 1;

			auto* loBound = &m_chunks.front();
			auto* hiBound = &m_chunks.back() + 1;

			if (hi == hiBound)
			{
				hi = nullptr;
			}

			for (;;)
			{
				if (lo != nullptr)
				{
					if (p >= lo->m_data && p < lo->m_data + chunkLength)
					{
						return lo;
					}

					if (lo == loBound)
					{
						lo = nullptr;
					}
					else
					{
						--lo;
					}
				}

				if (hi != nullptr)
				{
					if (p >= hi->m_data && p < hi->m_data + chunkLength)
					{
						return hi;
					}

					if (++hi == hiBound)
					{
						hi = nullptr;
					}
				}
			}
		}


		std::size_t m_blockSize;
		unsigned char m_numBlocks;
		std::vector<Chunk> m_chunks;
		Chunk* m_allocChunk;
		Chunk* m_deallocChunk;
		mutable const SmallObjectFixedAllocator* m_prev;
		mutable const SmallObjectFixedAllocator* m_next;
	};
}

struct VeritasEngine::SmallObjectAllocator::Impl 
{
	Impl()
		: m_pool{}, m_lastAlloc{ nullptr }, m_lastDealloc{ nullptr }, m_chunkSize{ 0 }, m_maxObjectSize{ 0 }
	{

	}

	std::vector<SmallObjectFixedAllocator> m_pool;
	SmallObjectFixedAllocator* m_lastAlloc;
	SmallObjectFixedAllocator* m_lastDealloc;
	std::size_t m_chunkSize;
	std::size_t m_maxObjectSize;
};

VeritasEngine::SmallObjectAllocator::SmallObjectAllocator(std::size_t chunkSize, std::size_t maxObjectSize)
	: m_impl  { std::make_unique<Impl>() }
{
}

VeritasEngine::SmallObjectAllocator::~SmallObjectAllocator() = default;

void * VeritasEngine::SmallObjectAllocator::Allocate(std::size_t numBytes)
{
	if (numBytes > m_impl->m_maxObjectSize)
	{
		return operator new(numBytes);
	}

	if (m_impl->m_lastAlloc != nullptr && m_impl->m_lastAlloc->BlockSize() == numBytes)
	{
		return m_impl->m_lastAlloc->Allocate();
	}

	auto iter = std::lower_bound(m_impl->m_pool.begin(), m_impl->m_pool.end(), numBytes);

	if (iter == m_impl->m_pool.end() || iter->BlockSize() != numBytes)
	{
		iter = m_impl->m_pool.emplace(iter, numBytes);
		m_impl->m_lastDealloc = &*m_impl->m_pool.begin();
	}

	m_impl->m_lastAlloc = &*iter;

	return m_impl->m_lastAlloc->Allocate();
}

void VeritasEngine::SmallObjectAllocator::Deallocate(void * p, std::size_t size)
{
	if (size > m_impl->m_maxObjectSize)
	{
		return operator delete(p);
	}

	if (m_impl->m_lastDealloc != nullptr && m_impl->m_lastDealloc->BlockSize() == size)
	{
		m_impl->m_lastDealloc->Deallocate(p);
		return;
	}

	auto iter = std::lower_bound(m_impl->m_pool.begin(), m_impl->m_pool.end(), size);

	assert(iter != m_impl->m_pool.end());
	assert(iter->BlockSize() == size);

	m_impl->m_lastDealloc = &*iter;
	m_impl->m_lastDealloc->Deallocate(p);
}
