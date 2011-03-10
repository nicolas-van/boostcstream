
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_CSOURCE_HPP
#define BOOST_CSTREAM_CSOURCE_HPP

#include <iostream>
#include <string>
#include "encoding_traits.hpp"
#include "low/low_other.hpp"
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>

namespace boost {
namespace cstream {

    /**
        Base converting source.
    */
    template <typename CharType
            ,typename STrait=encoding_traits<CharType> >
    struct base_csource {
        typedef CharType char_type;
        struct category : boost::iostreams::source_tag {};
        typedef STrait encoding_trait;

        /**
            Constructor specifying the source stream and the
            read line policy. The source character set is
            the one used by the source stream by default.
        */
        template <typename IStream>
        base_csource(boost::reference_wrapper<IStream> stream
                ,reading_new_line_policy nlpol = read_auto) {
            init(stream,encoding_traits<
                typename IStream::char_type
                ,typename IStream::traits_type>::default_encoding()
                    ,nlpol);
        }

        /**
            Constructor specifuing the source stream, the source
            character set and the read line policy.
        */
        template <typename IStream>
        base_csource(boost::reference_wrapper<IStream> stream
                ,const std::string& origin
                ,reading_new_line_policy nlpol = read_auto) {
            init(stream,origin,nlpol);
        }

        std::streamsize read(char_type* s,std::streamsize n) {
            char* tmp = reinterpret_cast<char*>(s);
            std::streamsize readed = first->read(tmp,n*sizeof(char_type));
            return readed/sizeof(char_type);
        }

        private:

        template <typename IStream>
        void init(boost::reference_wrapper<IStream> stream
                ,const std::string& origin
                ,reading_new_line_policy nlpol) {
            final.reset(low::make_source(stream));
            third.reset(new low::utf16_from_anything(final.get()
                ,low::traduce_encoding(origin)));
            second.reset(new low::crlf_remover(NULL));
            first.reset(new low::anything_from_utf16(NULL
                ,encoding_trait::default_encoding()));

            if(!(third->input.get_min_size()%
                    sizeof(typename low::source_adapter<IStream>::char_type)==0))
                throw char_size_error("origin input stream "
                    "char size not appropriate");
            if(!(first->output.get_min_size()%sizeof(char_type)==0))
                throw char_size_error("current input stream "
                    "char size not appropriate");
            switch (nlpol) {
                case read_auto:
                    second->from = third.get();
                    first->from = second.get();
                break;
                case read_no_change:
                    first->from = third.get();
                break;
                default:
                    throw std::logic_error("unsupported new line policy");
                break;
            }
        }

        boost::shared_ptr<low::source_interface > final;
        boost::shared_ptr<low::utf16_from_anything> third;
        boost::shared_ptr<low::crlf_remover> second;
        boost::shared_ptr<low::anything_from_utf16> first;
    };

    typedef base_csource<char> csource;

    typedef boost::iostreams::stream_buffer<csource> icstream_buffer;
    typedef boost::iostreams::stream<csource> icstream;

#ifdef BOOST_CSTREAM_USE_WCHAR_T
    typedef base_csource<wchar_t> wcsource;

    typedef boost::iostreams::stream_buffer<wcsource> wicstream_buffer;
    typedef boost::iostreams::stream<wcsource> wicstream;
#endif


}
}

#endif // BOOST_CSTREAM_CSOURCE_HPP
