
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_LOW_GENERAL_HPP
#define BOOST_CSTREAM_LOW_GENERAL_HPP

#include <boost/utility.hpp>

namespace boost {
namespace cstream {
namespace low {

    struct sink_interface : boost::noncopyable {

        virtual bool flush() = 0;

        virtual std::streamsize write(const char* s, std::streamsize n) = 0;

        virtual ~sink_interface() {};
    };

    struct source_interface : boost::noncopyable {

        virtual std::streamsize read(char* s, std::streamsize n) = 0;

        virtual ~source_interface() {};
    };
}
}
}

#endif // GENERAL_HPP_INCLUDED
