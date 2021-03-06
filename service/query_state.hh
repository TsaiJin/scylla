
/*
 * Copyright (C) 2015 ScyllaDB
 */

/*
 * This file is part of Scylla.
 *
 * Scylla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Scylla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Scylla.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SERVICE_QUERY_STATE_HH
#define SERVICE_QUERY_STATE_HH

#include "service/client_state.hh"
#include "tracing/tracing.hh"

namespace service {

class query_state final {
private:
    client_state _client_state;
    tracing::trace_state_ptr _trace_state_ptr;

public:
    query_state(client_state client_state)
        : _client_state(client_state)
        , _trace_state_ptr(_client_state.trace_state_ptr())
    { }

    void begin_tracing(sstring request, gms::inet_address client, std::unordered_map<sstring, sstring> params) {
        tracing::begin(_trace_state_ptr, std::move(request), client, std::move(params));
    }

    bool is_tracing() const {
        return (bool)_trace_state_ptr;
    }

    const utils::UUID& tracing_session_id() const {
        return _trace_state_ptr->get_session_id();
    }

    tracing::trace_type trace_type() const {
        return _trace_state_ptr->get_type();
    }

    bool flush_trace_on_close() const {
        return _trace_state_ptr->get_flush_on_close();
    }

    void trace(const sstring& message) {
        tracing::trace(_trace_state_ptr, std::move(message));
    }

    client_state& get_client_state() {
        return _client_state;
    }

    const client_state& get_client_state() const {
        return _client_state;
    }
    api::timestamp_type get_timestamp() {
        return _client_state.get_timestamp();
    }
};

}

#endif
