#pragma once

#include <functional>
#include <future>
#include <vector>

namespace wf::utils {
    template <typename TItem, typename TComputed>
    class DistributedContainer {
    public:
        explicit DistributedContainer(std::size_t batchSize);

        void addItem(TItem &&item);

        using ItemHandler = std::function<void (TItem &item)>;
        void iterItems(ItemHandler const &handler);

        using ItemHandlerParallel = std::function<void (TItem &item, std::back_insert_iterator<std::vector<TComputed>> computedInserter)>;
        void iterItemsParallel(ItemHandlerParallel const &handler);

        using ComputedHandler = std::function<void (TComputed const &computedItem)>;
        void iterComputed(ComputedHandler const &handler) const;

        void resetComputed();

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
        std::vector<std::vector<TComputed>> m_computed;

        std::vector<std::future<void>> m_futures;
    };
}

///////////////////////////
// Implementation
//

template <typename TItem, typename TComputed>
wf::utils::DistributedContainer<TItem, TComputed>::DistributedContainer(std::size_t batchSize):
    BATCH_SIZE(batchSize)
{}

template <typename TItem, typename TComputed>
void wf::utils::DistributedContainer<TItem, TComputed>::addItem(TItem &&item) {
    m_items.emplace_back(std::forward<TItem &&>(item));

    if (m_items.size() % BATCH_SIZE == 1) {
        m_computed.emplace_back(std::vector<TComputed>{});
    }
}

template <typename TItem, typename TComputed>
void wf::utils::DistributedContainer<TItem, TComputed>::resetComputed() {
    for (auto &batch : m_computed) {
        batch.clear();
    }
}

template <typename TItem, typename TComputed>
void wf::utils::DistributedContainer<TItem, TComputed>::iterItems(ItemHandler const &handler) {
    for (auto &item : m_items) {
        handler(item);
    }
}

template <typename TItem, typename TComputed>
void wf::utils::DistributedContainer<TItem, TComputed>::iterItemsParallel(ItemHandlerParallel const &handler) {
    m_futures.clear();

    for (std::size_t batchIdx = 0; batchIdx < m_computed.size(); ++batchIdx) {
        m_futures.emplace_back(std::async(std::launch::async, [this, batchIdx, &handler]() {
            auto inserter = std::back_inserter(m_computed[batchIdx]);
            auto itemsBegin = std::begin(m_items) + batchIdx * BATCH_SIZE;
            auto itemsEnd = m_items.size() < (batchIdx + 1) * BATCH_SIZE ? std::end(m_items) : itemsBegin + BATCH_SIZE;

            for (auto itemIterator = itemsBegin; itemIterator != itemsEnd; ++itemIterator) {
                handler(*itemIterator, inserter);
            }
        }));
    }

    for (auto &future : m_futures) {
        future.wait();
    }
}

template <typename TItem, typename TComputed>
void wf::utils::DistributedContainer<TItem, TComputed>::iterComputed(ComputedHandler const &handler) const {
    for (auto const &batch : m_computed) {
        for (auto const &computed : batch) {
            handler(computed);
        }
    }
}
