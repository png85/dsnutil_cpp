#pragma once

#include <dsnutil/event/broadcast_channel.hpp>

namespace dsn {
namespace event {
    /// \brief Interface for broadcast event handlers
    ///
    /// Implement this to have an object register/unregister itself as handler for
    /// broadcasted \p Tmessage events during its lifetime.
    ///
    /// \tparam Tmessage Event type for which the object shall register itself as a broadcast handler
    ///
    /// \see broadcast_channel
    template <typename Tmessage> class broadcast_handler {
    public:
        broadcast_handler();
        ~broadcast_handler();

        /// \brief Broadcast event callback
        ///
        /// This will be invoked by the event system whenever a \p Tmessage type event occurs.
        ///
        /// \param message Reference to the structure containing the brodcast event's data
        virtual void operator()(const Tmessage& message) = 0;
    };

    /// \brief Register ourself as broadcast handler
    ///
    /// This registers the object as a handler for \p Tm type broadcasts with the event system.
    template <typename Tm> broadcast_handler<Tm>::broadcast_handler() { broadcast_channel::add_handler<Tm>(this); }

    /// \brief Unregister ourself as broadcast handler
    ///
    /// This uregisters the object as a handler for \p Tm type broadcasts.
    template <typename Tm> broadcast_handler<Tm>::~broadcast_handler() { broadcast_channel::remove_handler<Tm>(this); }
}
}
