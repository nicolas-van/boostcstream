
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_DETAILS_HPP
#define BOOST_CSTREAM_DETAILS_HPP

#include <boost/config.hpp>

/**
    Define if this library will provide support
    for wchar_t type. Defaults to yes if
    wchar_t is available.
*/
#ifndef BOOST_CSTREAM_USE_WCHAR_T
#if !defined(BOOST_NO_CWCHAR)
#define BOOST_CSTREAM_USE_WCHAR_T
#endif
#endif

namespace boost {
namespace cstream {

    /**
        Enumeration to specify the way a converting stream will
        handle new lines while reading.
    */
    enum reading_new_line_policy {
        /**
            Convert any CR or CR LF to LF.
        */
        read_auto,
        /**
            Do not change anything.
        */
        read_no_change
    };

    /**
        Enumeration to specify the way a converting stream will
        handle new lines while writing.
    */
    enum writing_new_line_policy {
        /**
            Try to detect the current platform
            convention and use it.
        */
        write_auto,
        /**
            Each new line is expressed with a single
            LF (which means the nothing is changed).
        */
        write_lf,
        /**
            Each new line is expressed with a CR
            followed by a LF.
        */
        write_crlf,
        /**
            Each new line is expressed with a single
            CR.
        */
        write_cr
    };

}
}

#endif
