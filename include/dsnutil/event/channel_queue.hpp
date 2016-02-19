#pragma once

#include <algorithm>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <vector>

#include <dsnutil/singleton.h>

namespace dsn {
namespace event {

    /// \brief Queue implementation for broadcast channels
    ///
    /// This template provides the facilities to register/unregister handlers for \p Tmessage type
    /// broadcast events as well as sending a broadcast to all currently registered handler objects
    /// that implement the corresponding \a broadcast_handler templated interface.
    ///
    /// \internal This shouldn't be used directly since the Singleton API is painful. Use the
    /// simplified interface that is provided through \a broadcast_channel instead.
    ///
    /// \see broadcast_channel
    /// \see broadcast_handler
    template <typename Tmessage> class channel_queue : public dsn::Singleton<channel_queue<Tmessage> > {
        friend class dsn::Singleton<channel_queue<Tmessage> >;

    private:
        channel_queue() = default;

        /// \brief Type alias for handler functions
        ///
        /// \internal This is used internally since we wrap each templated call to a handler in
        /// a lambda and then put that into \a m_handlers
        using handler_type = std::function<void(const Tmessage&)>;

        /// \brief Type alias for mutex
        ///
        /// This is used to alias the mutex implementation for synchronizing access to the handler
        /// queue.
        using mutex_type = std::mutex;

        /// \brief Type alias for scoped mutex lock
        ///
        /// This is used to alias a scoped mutex lock for \a mutex_type.
        using scoped_lock = std::lock_guard<mutex_type>;

        /// \brief Mutex for queue access
        ///
        /// This is used to synchronize access to the handler queue from different threads.
        ///
        /// \note This needs to be mutable since we require mutex locking in \p const methods too.
        mutable mutex_type m_mutex;

        /// \brief Event handlers
        ///
        /// This contains the auto-generated lambdas which invoke a registered event handler.
        std::vector<handler_type> m_handlers;

        /// \brief Pointers to registered handler objects
        ///
        /// This contains pointers to all currently registered \a broadcast_handler objects
        std::vector<void*> m_original_pointers;

    public:
        template <typename Thandler> void add_handler(Thandler* handler);
        template <typename Thandler> void remove_handler(Thandler* handler);
        void clear_handlers();
        size_t num_handlers() const;
        void broadcast(const Tmessage& message);
    };

    /// \brief Add handler to channel
    ///
    /// This adds a handler object of type \p Th to the channel queue for events of type \p Tm.
    ///
    /// \param handler Pointer to the handler that shall be called on \p Tm events
    ///
    /// \tparam Tm Event's message type
    /// \tparam Th Handler object's type
    ///
    /// \throw std::runtime_error if the user tries to register the same handler multiple times
    template <typename Tm> template <typename Th> void channel_queue<Tm>::add_handler(Th* handler)
    {
        scoped_lock guard(m_mutex);
        auto it = std::find(m_original_pointers.begin(), m_original_pointers.end(), handler);
        if (it != m_original_pointers.end()) {
            throw std::invalid_argument("Tried to add the same handler object multiple times!");
        }

        m_handlers.push_back([handler](const Tm& message) { (*handler)(message); });
        m_original_pointers.push_back(handler);
    }

    /// \brief Remove handler form channel
    ///
    /// This removes a handler object of type \p Th from the channel queue for events of type \p Tm.
    ///
    /// \param handler Pointer to the handler that shall be removed from the queue for \p Tm broadcasts.
    ///
    /// \tparam Tm Event's message type
    /// \tparam Th Handler object's type
    template <typename Tm> template <typename Th> void channel_queue<Tm>::remove_handler(Th* handler)
    {
        scoped_lock guard(m_mutex);
        auto it = std::find(m_original_pointers.begin(), m_original_pointers.end(), handler);
        if (it == m_original_pointers.end()) {
            throw std::invalid_argument("Tried to remove a handler object that wasn't registered!");
        }

        auto index = it - std::begin(m_original_pointers);
        m_handlers.erase(m_handlers.begin() + index);
        m_original_pointers.erase(it);
    }

    /// \brief Clear all handlers for a given message type
    ///
    /// Removes all handlers currently installed for \p Tm type broadcasts.
    template <typename Tm> void channel_queue<Tm>::clear_handlers()
    {
        scoped_lock guard(m_mutex);
        m_handlers.clear();
        m_original_pointers.clear();
    }

    /// \brief Get numbers of handlers for a given message type
    ///
    /// \return Number of handlers currently installed for \p Tm type broadcasts
    template <typename Tm> size_t channel_queue<Tm>::num_handlers() const
    {
        scoped_lock guard(m_mutex);
        return m_handlers.size();
    }

    /// \brief Broadcast message to all registered handlers
    ///
    /// Broadcasts a \p Tm type \a message to all handlers currently registered.
    ///
    /// \note This works with a copy of \p m_handlers and allows modifications to the handler list from
    /// other threads while handlers are being executed in the current one. Any changes made by the
    /// executed handlers will be in effect starting with the next call to this method and not corrupt
    /// the list for the current call.
    template <typename Tm> void channel_queue<Tm>::broadcast(const Tm& message)
    {
        // create thread-local copy of active handlers so our queue can be changed while
        // we execute them without missing a currently installed handler
        std::vector<handler_type> handlers;
        {
            scoped_lock guard(m_mutex);
            handlers.reserve(m_handlers.size());
            handlers = m_handlers;
        }

        // execute all installed handlers
        for (auto& handler : handlers) {
            handler(message);
        }
    }
}
}
