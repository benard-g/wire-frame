#pragma once

#include <functional>
#include <future>
#include <vector>

namespace wf::utils {
    template <typename TItem, typename TCache>
    class DistributedContainer {
    public:
        explicit DistributedContainer(std::size_t batchSize);

        void addItem(TItem &&item);

        using ItemHandler = std::function<void (TItem &item)>;
        void iterItems(ItemHandler const &handler);

        using ItemHandlerParallel = std::function<void (TItem &item, std::back_insert_iterator<std::vector<TCache>> cacheInserter)>;
        void iterItemsParallel(ItemHandlerParallel const &handler);

        using CachedHandler = std::function<void (TCache const &cachedItem)>;
        void iterCached(CachedHandler const &handler) const;

        void resetCache();

    public:
        // Coplien
        DistributedContainer() = delete;
        ~DistributedContainer() noexcept = default;
        DistributedContainer(DistributedContainer const &) = delete;
        DistributedContainer &operator=(DistributedContainer const &) = delete;
        DistributedContainer(DistributedContainer &&) noexcept = delete;
        DistributedContainer &operator=(DistributedContainer &&) noexcept = delete;

    private:
        std::size_t const BATCH_SIZE;

        std::vector<TItem> m_items;
        std::vector<std::vector<wf::core::Line>> m_cached;
    };
}

///////////////////////////
// Implementation
//

template <typename TItem, typename TCache>
wf::utils::DistributedContainer<TItem, TCache>::DistributedContainer(std::size_t batchSize):
    BATCH_SIZE(batchSize)
{}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::addItem(TItem &&item) {
    m_items.emplace_back(std::forward<TItem &&>(item));

    if (m_items.size() % BATCH_SIZE == 1) {
        m_cached.emplace_back(std::vector<TCache>{});
    }
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::resetCache() {
    for (auto &batch : m_cached) {
        batch.clear();
    }
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::iterItems(ItemHandler const &handler) {
    for (auto &item : m_items) {
        handler(item);
    }
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::iterItemsParallel(ItemHandlerParallel const &handler) {
    std::vector<std::future<void>> futures;

    for (std::size_t batchIdx = 0; batchIdx < m_cached.size(); ++batchIdx) {
        futures.emplace_back(std::async(std::launch::async, [this, batchIdx, handler]() {
            auto inserter = std::back_inserter(m_cached[batchIdx]);
            auto itemsBegin = std::begin(m_items) + batchIdx * BATCH_SIZE;
            auto itemsEnd = m_items.size() < (batchIdx + 1) * BATCH_SIZE ? std::end(m_items) : itemsBegin + BATCH_SIZE;

            for (auto itemIterator = itemsBegin; itemIterator != itemsEnd; ++itemIterator) {
                handler(*itemIterator, inserter);
            }
        }));
    }

    for (auto &future : futures) {
        future.wait();
    }
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::iterCached(CachedHandler const &handler) const {
    for (auto const &batch : m_cached) {
        for (auto const &cached : batch) {
            handler(cached);
        }
    }
}
