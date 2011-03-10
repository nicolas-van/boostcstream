
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_EXCEPTIONS_HPP
#define BOOST_CSTREAM_EXCEPTIONS_HPP

#include <stdexcept>

namespace boost{
namespace cstream{

    /**
        Thrown when an error occurs before or during
        conversion.
    */
    struct conversion_error : std::runtime_error {
        conversion_error()
                :std::runtime_error("unknown error while converting") {}
        conversion_error(const std::string& mes)
                :std::runtime_error(mes) {}
    };

    /**
        Thrown when the current platform is not correctly
        supported.
    */
    struct platform_error : std::logic_error {
        platform_error(const std::string& mes)
                :std::logic_error(mes) {}
    };

    /**
        Thrown when the size of a stream is not appropriate
        for the chosen character set.
    */
    struct char_size_error : std::logic_error {
        char_size_error(const std::string& mes)
                :std::logic_error(mes) {}
    };

}
}

#endif
