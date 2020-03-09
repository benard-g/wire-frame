#pragma once

#include <functional>
#include <vector>

namespace wf::utils {
    template <typename TItem, typename TCache>
    class DistributedContainer {
    public:
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
        DistributedContainer() = default;
        ~DistributedContainer() noexcept = default;
        DistributedContainer(DistributedContainer const &) = delete;
        DistributedContainer &operator=(DistributedContainer const &) = delete;
        DistributedContainer(DistributedContainer &&) noexcept = delete;
        DistributedContainer &operator=(DistributedContainer &&) noexcept = delete;

    private:
        std::vector<TItem> m_items;
        std::vector<TCache> m_cached;
    };
}

///////////////////////////
// Implementation
//

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::addItem(TItem &&item) {
    m_items.emplace_back(std::forward<TItem &&>(item));
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::resetCache() {
    m_cached.clear();
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::iterItems(ItemHandler const &handler) {
    for (auto &item : m_items) {
        handler(item);
    }
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::iterItemsParallel(ItemHandlerParallel const &handler) {
    auto inserter = std::back_inserter(m_cached);

    // TODO use threads for parallelization
    for (auto &item : m_items) {
        handler(item, inserter);
    }
}

template <typename TItem, typename TCache>
void wf::utils::DistributedContainer<TItem, TCache>::iterCached(CachedHandler const &handler) const {
    for (auto const &cached : m_cached) {
        handler(cached);
    }
}
