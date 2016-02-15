#pragma once

#include <type_traits>

#include <dsnutil/event/channel_queue.hpp>

namespace dsn {
namespace event {

    template <typename Tmessage> class broadcast_handler;

    /// \brief Metaprogramming helper for is_broadcast_handler
    ///
    /// This can be used to check whether \p Thandler implements \p broadcast_handler<T> during compile time.
    template <typename Tmessage, typename Thandler> struct is_broadcast_handler {
        static const bool value = std::is_base_of<broadcast_handler<Tmessage>, Thandler>::value;
    };

    /// \brief Channel for broadcast-style events
    ///
    /// This provides an interface to the \a channel_queue that is used to broadcast events to
    /// objects implementing the \a broadcast_handler interface.
    ///
    /// Broadcasted events can be of arbitrary types as long as the handler object implements the
    /// proper \a broadcast_handler templated interface.
    ///
    /// \note The queue mechanism for events is actually implemented in \a channel_queue and this
    /// class mainly shields the user from having to use the singleton instance methods every time
    /// they want to utilize the broadcast system.
    ///
    /// \see channel_queue
    /// \see broadcast_handler
    class broadcast_channel {
    public:
        template <typename Tmessage, typename Thandler> static void add_handler(Thandler* handler);
        template <typename Tmessage, typename Thandler> static void remove_handler(Thandler* handler);
        template <typename Tmessage> static void clear_handlers();
        template <typename Tmessage> static size_t num_handlers();
        template <typename Tmessage> static void broadcast(const Tmessage& message);
    };

    /// \brief Add object to handler queue for broadcast events
    ///
    /// Adds \a handler as a handler for \p Tm type broadcasts
    ///
    /// \param handler Pointer to the handler object that shall be added
    ///
    /// \note This only works with \p Th objects that implement the \a broadcast_handler interface for \p Tm!
    ///
    /// \tparam Tm Message type for which \a handler shall be registered
    /// \tparam Th Type of the handler object that shall be registered
    ///
    /// \throw std::invalid_argument if \a handler is already registered for \p Tm type broadcasts
    template <typename Tm, typename Th> void broadcast_channel::add_handler(Th* handler)
    {
        static_assert(is_broadcast_handler<Tm, Th>::value,
            "broadcast_channel::add_handler<Tm, Th>() only works with Th's that implement broadcast_handler<Tm>");
        channel_queue<Tm>::instancePtr()->add_handler(handler);
    }

    /// \brief Remove object from handler queue for broadcast events
    ///
    /// Removes \a handler from the queue for \p Tm type broadcasts
    ///
    /// \param handler Pointer to the handler object that shall be removed
    ///
    /// \param note This only works with \p Th objects that implement the \a broadcast_handler interface for \p Tm!
    ///
    /// \tparam Tm Message type for which \a handler shall be unregistered
    /// \tparam Th Type of the handler object that shall be unregistered
    ///
    /// \throw std::invalid_argument if \a handler isn't registered for \p Tm type broadcasts
    template <typename Tm, typename Th> void broadcast_channel::remove_handler(Th* handler)
    {
        static_assert(is_broadcast_handler<Tm, Th>::value,
            "broadcast_channel::remove_handler<Tm, Th>() only works with Th's that implement broadcast_handler<Tm>");
        channel_queue<Tm>::instancePtr()->remove_handler(handler);
    }

    /// \brief Clear all handlers for a broadcast type
    ///
    /// This removes all registered handler objects for \p Tm type broadcasts.
    ///
    /// \tparam Tm Message type for which all handlers shall be discarded
    template <typename Tm> void broadcast_channel::clear_handlers()
    {
        channel_queue<Tm>::instancePtr()->clear_handlers();
    }

    /// \brief Get number of handlers for broadcast type
    ///
    /// Queries how many objects are currently registered as handlers for \p Tm type broadcasts.
    ///
    /// \tparam Tm Message type for which the number of handlers shall be queried
    ///
    /// \return Number of handlers currently installed for \p Tm type broadcasts
    template <typename Tm> size_t broadcast_channel::num_handlers()
    {
        return channel_queue<Tm>::instancePtr()->num_handlers();
    }

    /// \brief Broadcast an event
    ///
    /// This broadcasts the given \a message to all handlers that are currently registered for \p Tm
    /// type broadcasts.
    ///
    /// \param message Reference to the message that shall be broadcasted
    ///
    /// \tparam Tm Message type that shall be broadcasted
    template <typename Tm> void broadcast_channel::broadcast(const Tm& message)
    {
        channel_queue<Tm>::instancePtr()->broadcast(message);
    }
}
}
