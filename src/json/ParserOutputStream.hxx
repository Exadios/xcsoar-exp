/*
 * Copyright 2021 Max Kellermann <max.kellermann@gmail.com>
 *
 * author: Max Kellermann <mk@cm4all.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * FOUNDATION OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "io/OutputStream.hxx"

#pragma GCC diagnostic push
/*
 * The following pragma solves a error in boost lib v1.80.0. The error
 * presents as:
 * "boost_1_80_0/boost/system/detail/error_code.hpp:60:13: error: redundant redeclaration of ‘std::size_t boost::system::hash_value(const error_code&)’ in same scope [-Werror=redundant-decls]"
 * Why the boost error is occurring is not clear to me at this time. Also how
 * it ever work is, likewise, not clear.
 */
#pragma GCC diagnostic ignored "-Wredundant-decls"
#include <boost/json.hpp>
#pragma GCC diagnostic pop

namespace Json {

/**
 * An #OutputStream implementation which parses all incoming data as
 * JSON.  Call Finish() to obtain the JSON value.
 */
class ParserOutputStream final : public OutputStream {
	boost::json::stream_parser parser;

public:
	decltype(auto) Finish() {
		return parser.release();
	}

	/* virtual methods from class OutputStream */
	void Write(const void *data, size_t size) override;
};

} // namespace Json
