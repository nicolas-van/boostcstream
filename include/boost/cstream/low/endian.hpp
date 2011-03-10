
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_LOW_ENDIAN_HPP
#define BOOST_CSTREAM_LOW_ENDIAN_HPP

#include <boost/cstdint.hpp>

namespace boost {
namespace cstream {
namespace low {

    inline bool is_2_bytes_little_endian() {
        boost::uint16_t tmp = 1;
        return *(reinterpret_cast<boost::uint8_t*>(&tmp))!=0;
    }

    inline bool is_4_bytes_little_endian() {
        boost::uint32_t tmp = 1;
        return *(reinterpret_cast<boost::uint8_t*>(&tmp))!=0;
    }

}
}
}

#endif
