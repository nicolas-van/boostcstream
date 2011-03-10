
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_LOW_LOW_HPP
#define BOOST_CSTREAM_LOW_LOW_HPP

#include <boost/utility.hpp>
#include <string>
#include <vector>
#include "../details.hpp"
#include "../exceptions.hpp"
#include "general.hpp"


namespace boost {
namespace cstream {
namespace low {

    /**
        Utility class to create an ICU converter.
    */
    struct converter_base : boost::noncopyable {
        converter_base(const std::string& charset);

        ~converter_base();

        size_t get_min_size();

        void* real;
    };

    /**
        UTF16 without BOM dependant of the platform
        endianess.
    */
    struct anything_to_utf16 : public sink_interface {
        anything_to_utf16(sink_interface* redirect
            ,const std::string& source) : dest(redirect),
            input(source) {}

        std::streamsize write(const char* s, std::streamsize n);

        bool flush() {dest->flush();
            return true;};

        sink_interface* dest;
        converter_base input;
    };

    /**
        UTF16 without BOM dependant of the platform
        endianess.
    */
    struct utf16_to_anything : public sink_interface {
        utf16_to_anything(sink_interface* redirect
            ,const std::string& destination) : dest(redirect),
            output(destination) {}

        std::streamsize write(const char* s, std::streamsize n);

        bool flush() {dest->flush();
            return true;};

        sink_interface* dest;
        converter_base output;
    };

    /**
        UTF16 without BOM dependant of the platform
        endianess.
    */
    struct utf16_from_anything : public source_interface {
        utf16_from_anything(source_interface* redirected
            ,const std::string& source) : from(redirected),
            input(source),tmp_begin(0),tmp_end(0) {}

        std::streamsize read(char* s, std::streamsize n);

        source_interface* from;
        converter_base input;
        std::vector<char> temporary;
        size_t tmp_begin;
        size_t tmp_end;
    };

    /**
        UTF16 without BOM dependant of the platform
        endianess.
    */
    struct anything_from_utf16 : public source_interface {
        anything_from_utf16(source_interface* redirected
            ,const std::string& destination) : from(redirected),
            output(destination),tmp_begin(0),tmp_end(0) {}

        std::streamsize read(char* s, std::streamsize n);

        source_interface* from;
        converter_base output;
        std::vector<char> temporary;
        size_t tmp_begin;
        size_t tmp_end;
    };

    struct crlf_remover : public source_interface {
        crlf_remover(source_interface* redirected)
            :from(redirected),lastcr(false) {}

        std::streamsize read(char* s, std::streamsize n);

        source_interface* from;
        bool lastcr;
    };

    struct lf_to_crlf_converter : public sink_interface {
        lf_to_crlf_converter(sink_interface* redirect)
            :to(redirect) {}

        std::streamsize write(const char* s, std::streamsize n);

        bool flush() {to->flush();
            return true;};

        sink_interface* to;
    };

    struct lf_to_cr_converter : public sink_interface {
        lf_to_cr_converter(sink_interface* redirect)
            :to(redirect) {}

        std::streamsize write(const char* s, std::streamsize n);

        bool flush() {to->flush();
            return true;};

        sink_interface* to;
    };

    size_t get_ICU_char_size();

    std::string get_char_encoding();

#ifdef BOOST_CSTREAM_USE_WCHAR_T
    std::string get_wchar_encoding();
#endif

    boost::cstream::writing_new_line_policy get_new_line_policy();

    // all UTF have no aliases, so it's possible to use this
    // function to compare them
    // standard name of latin1 is ISO-8859-1
    bool equals_charset(const std::string& one,const std::string& two);


    inline std::string traduce_encoding(const std::string& set) {
        if(equals_charset("",set))
            return get_char_encoding();
        return set;
    }

}
}
}

#endif

