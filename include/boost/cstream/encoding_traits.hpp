
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_ENCODING_TRAITS_HPP
#define BOOST_CSTREAM_ENCODING_TRAITS_HPP

#include "details.hpp"
#include "low/low_icu.hpp"

namespace boost {
namespace cstream {

    /**
        Trait class to indicate the assignated character
        encoding of a pair type + std::char_traits.
    */
    template <typename CharType
            ,typename Trait=std::char_traits<CharType> >
    struct encoding_traits {};

    template <>
    struct encoding_traits<char,std::char_traits<char> > {
        typedef char char_type;
        typedef std::char_traits<char_type> char_trait;

        static std::string default_encoding() {
            return low::get_char_encoding();
        }

    };

#ifdef BOOST_CSTREAM_USE_WCHAR_T
    template <>
    struct encoding_traits<wchar_t,std::char_traits<wchar_t> > {
        typedef wchar_t char_type;
        typedef std::char_traits<char_type> char_trait;

        static std::string default_encoding() {
            return low::get_wchar_encoding();
        }

    };
#endif

}
}

#endif // BOOST_CSTREAM_ENCONDING_TRAITS_HPP
