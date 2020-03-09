#pragma once

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace wf::core {
    class EventManager {
    public:
        template <typename TEvent>
        using Callback = std::function<void (TEvent const &)>;

        /*!
         * Subscribe a new callback to an Event.
         *
         * @tparam TEvent   The type of Event.
         * @param callback  An event handler that will be called every time the Event occurs.
         */
        template <typename TEvent>
        void subscribe(Callback<TEvent> callback);

        /*!
         * Broadcast an Event by calling all the subscribed callbacks.
         *
         * @tparam TEvent   The type of Event.
         * @param event     The Event instance.
         */
        template <typename TEvent>
        void notify(const TEvent &event);

    public:
        // Coplien
        EventManager() = default;
        ~EventManager() noexcept = default;
        EventManager(EventManager const &) = delete;
        EventManager &operator=(EventManager const &) = delete;
        EventManager(EventManager &&) noexcept = delete;
        EventManager &operator=(EventManager &&) noexcept = delete;

    private:
        class ICallbackWrapper;

        template <typename TEvent>
        class CallbackWrapper;

        std::unordered_multimap<std::type_index, std::unique_ptr<ICallbackWrapper>> m_subscriptions;
    };
}

////////////////////////////////
// Implementations
//

class wf::core::EventManager::ICallbackWrapper {
public:
    virtual ~ICallbackWrapper() noexcept = default;
};

template <typename TEvent>
class wf::core::EventManager::CallbackWrapper: public ICallbackWrapper {
public:
    explicit CallbackWrapper(Callback<TEvent> &&callback):
        m_callback(std::move(callback))
    {}

    ~CallbackWrapper() noexcept override = default;

    void invoke(TEvent const &event) {
        m_callback(event);
    }

private:
    Callback<TEvent> m_callback;
};

template <typename TEvent>
void wf::core::EventManager::subscribe(Callback<TEvent> callback) {
    m_subscriptions.emplace(
        typeid(TEvent),
        std::make_unique<CallbackWrapper<TEvent>>(std::move(callback))
    );
}

template <typename TEvent>
void wf::core::EventManager::notify(const TEvent &event) {
    auto range = m_subscriptions.equal_range(typeid(event));

    for (auto iterator = range.first; iterator != range.second; ++iterator) {
        auto &callback = static_cast<CallbackWrapper<TEvent> &>(*iterator->second);
        callback.invoke(event);
    }
}
