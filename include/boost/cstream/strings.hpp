
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_STRINGS_HPP
#define BOOST_CSTREAM_STRINGS_HPP

#include "gen_char_traits.hpp"
#include "encoding_traits.hpp"
#include "low/endian.hpp"
#include "cstream.hpp"
#include <string>
#include <boost/cstdint.hpp>
#include <sstream>


namespace boost {
namespace cstream {

    /*
        UTF-8 declarations
    */

    struct utf8_char_traits : gen_char_traits<char> {};

    typedef std::basic_ios<char,utf8_char_traits> utf8_ios;
    typedef std::basic_string<char,utf8_char_traits> utf8_string;
    typedef std::basic_streambuf<char,utf8_char_traits> utf8_streambuf;
    typedef std::basic_istream<char,utf8_char_traits> utf8_istream;
    typedef std::basic_ostream<char,utf8_char_traits> utf8_ostream;
    typedef std::basic_iostream<char,utf8_char_traits> utf8_iostream;
    typedef std::basic_stringbuf<char,utf8_char_traits> utf8_stringbuf;
    typedef std::basic_istringstream<char,utf8_char_traits> utf8_istringstream;
    typedef std::basic_ostringstream<char,utf8_char_traits> utf8_ostringstream;
    typedef std::basic_stringstream<char,utf8_char_traits> utf8_stringstream;

    template<>
    struct encoding_traits<char,utf8_char_traits > {
        typedef char char_type;
        typedef utf8_char_traits char_trait;

        static std::string default_encoding() {
            return "utf8";
        }
    };
    typedef base_csource<char,utf8_char_traits> utf8_csource;
    typedef boost::iostreams::stream_buffer<utf8_csource> utf8_icstream_buffer;
    typedef boost::iostreams::stream<utf8_csource> utf8_icstream;
    typedef base_csink<char,utf8_char_traits> utf8_csink;
    typedef boost::iostreams::stream_buffer<utf8_csink> utf8_ocstream_buffer;
    typedef boost::iostreams::stream<utf8_csink> utf8_ocstream;

    /*
        UTF-16 declarations
    */

    struct utf16_char_traits : gen_char_traits<boost::uint16_t> {};

    typedef std::basic_ios<boost::uint16_t,utf16_char_traits> utf16_ios;
    typedef std::basic_string<boost::uint16_t,utf16_char_traits> utf16_string;
    typedef std::basic_streambuf<boost::uint16_t,utf16_char_traits> utf16_streambuf;
    typedef std::basic_istream<boost::uint16_t,utf16_char_traits> utf16_istream;
    typedef std::basic_ostream<boost::uint16_t,utf16_char_traits> utf16_ostream;
    typedef std::basic_iostream<boost::uint16_t,utf16_char_traits> utf16_iostream;
    typedef std::basic_stringbuf<boost::uint16_t,utf16_char_traits> utf16_stringbuf;
    typedef std::basic_istringstream<boost::uint16_t,utf16_char_traits> utf16_istringstream;
    typedef std::basic_ostringstream<boost::uint16_t,utf16_char_traits> utf16_ostringstream;
    typedef std::basic_stringstream<boost::uint16_t,utf16_char_traits> utf16_stringstream;

    template<>
    struct encoding_traits<boost::uint16_t,utf16_char_traits > {
        typedef boost::uint16_t char_type;
        typedef utf16_char_traits char_trait;

        static std::string default_encoding() {
            if(low::is_2_bytes_little_endian())
                return "utf16LE";
            else
                return "utf16BE";
        }
    };
    typedef base_csource<uint16_t,utf16_char_traits> utf16_csource;
    typedef boost::iostreams::stream_buffer<utf16_csource> utf16_icstream_buffer;
    typedef boost::iostreams::stream<utf16_csource> utf16_icstream;
    typedef base_csink<uint16_t,utf16_char_traits> utf16_csink;
    typedef boost::iostreams::stream_buffer<utf16_csink> utf16_ocstream_buffer;
    typedef boost::iostreams::stream<utf16_csink> utf16_ocstream;

    /*
        UTF_32 declarations
    */

    struct utf32_char_traits : gen_char_traits<boost::uint32_t> {};

    typedef std::basic_ios<boost::uint32_t,utf32_char_traits> utf32_ios;
    typedef std::basic_string<boost::uint32_t,utf32_char_traits> utf32_string;
    typedef std::basic_streambuf<boost::uint32_t,utf32_char_traits> utf32_streambuf;
    typedef std::basic_istream<boost::uint32_t,utf32_char_traits> utf32_istream;
    typedef std::basic_ostream<boost::uint32_t,utf32_char_traits> utf32_ostream;
    typedef std::basic_iostream<boost::uint32_t,utf32_char_traits> utf32_iostream;
    typedef std::basic_stringbuf<boost::uint32_t,utf32_char_traits> utf32_stringbuf;
    typedef std::basic_istringstream<boost::uint32_t,utf32_char_traits> utf32_istringstream;
    typedef std::basic_ostringstream<boost::uint32_t,utf32_char_traits> utf32_ostringstream;
    typedef std::basic_stringstream<boost::uint32_t,utf32_char_traits> utf32_stringstream;

    template<>
    struct encoding_traits<boost::uint32_t,utf32_char_traits > {
        typedef boost::uint32_t char_type;
        typedef utf32_char_traits char_trait;

        static std::string default_encoding() {
            if(low::is_4_bytes_little_endian())
                return "utf32LE";
            else
                return "utf32BE";
        }
    };
    typedef base_csource<uint32_t,utf32_char_traits> utf32_csource;
    typedef boost::iostreams::stream_buffer<utf32_csource> utf32_icstream_buffer;
    typedef boost::iostreams::stream<utf32_csource> utf32_icstream;
    typedef base_csink<uint32_t,utf32_char_traits> utf32_csink;
    typedef boost::iostreams::stream_buffer<utf32_csink> utf32_ocstream_buffer;
    typedef boost::iostreams::stream<utf32_csink> utf32_ocstream;

}
}


#endif
