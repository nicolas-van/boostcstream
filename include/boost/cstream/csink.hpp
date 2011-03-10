
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_CSINK_HPP
#define BOOST_CSTREAM_CSINK_HPP

#include <iostream>
#include <string>
#include "encoding_traits.hpp"
#include "low/low_other.hpp"
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/shared_ptr.hpp>

namespace boost {
namespace cstream {

    /**
        Base converting sink.
    */
    template <typename CharType
            ,typename STrait=encoding_traits<CharType> >
    struct base_csink {
        typedef CharType char_type;
        struct category : boost::iostreams::sink_tag
                ,boost::iostreams::flushable_tag {};
        typedef STrait encoding_trait;

        /**
            Constructor specifying the destination stream and
            the writing new line policy. The destination character
            set is the one used by the destination stream by default.
        */
        template <typename OStream>
        base_csink(boost::reference_wrapper<OStream> stream
                ,writing_new_line_policy nlpol = write_lf) {
            init(stream,encoding_traits<
                typename OStream::char_type
                ,typename OStream::traits_type>::default_encoding()
                    ,nlpol);
        }

        /**
            Constructor specifying the destination stream, the
            destination character set and the writing new line
            policy.
        */
        template <typename OStream>
        base_csink(boost::reference_wrapper<OStream> stream
                ,const std::string& destination
                ,writing_new_line_policy nlpol = write_lf) {
            init(stream,destination,nlpol);
        }

        std::streamsize write(const char_type* s,std::streamsize n) {
            const char* tmp = reinterpret_cast<const char*>(s);
            std::streamsize writed = first->write(tmp,n*sizeof(char_type));
            return writed/sizeof(char_type);
        }

        bool flush() {
            return final->flush();
        }

        private:

        template <typename OStream>
        void init(boost::reference_wrapper<OStream> stream
                ,const std::string& destination
                ,writing_new_line_policy nlpol) {
            final.reset(low::make_sink(stream));
            third.reset(new low::utf16_to_anything(final.get()
                    ,low::traduce_encoding(destination)));
            second_crlf.reset(new low::lf_to_crlf_converter(NULL));
            second_cr.reset(new low::lf_to_cr_converter(NULL));
            first.reset(new low::anything_to_utf16(NULL
                    ,encoding_trait::default_encoding()));

            if(!(third->output.get_min_size()%
                    sizeof(typename low::sink_adapter<OStream>::char_type)==0))
                throw char_size_error("destination output stream "
                    "char size not appropriate");
            if(!(first->input.get_min_size()%sizeof(char_type)==0))
                throw char_size_error("current output stream "
                    "char size not appropriate");

            writing_new_line_policy pol = nlpol==write_auto
                    ?low::get_new_line_policy():nlpol;

            switch (pol) {
                case write_lf:
                    first->dest = third.get();
                break;
                case write_cr:
                    first->dest = second_cr.get();
                    second_cr->to = third.get();
                break;
                case write_crlf:
                    first->dest = second_crlf.get();
                    second_crlf->to = third.get();
                break;
                default:
                    throw std::logic_error("unsupported new line policy");
                break;
            }
        }

        boost::shared_ptr<low::sink_interface > final;
        boost::shared_ptr<low::utf16_to_anything> third;
        boost::shared_ptr<low::lf_to_crlf_converter> second_crlf;
        boost::shared_ptr<low::lf_to_cr_converter> second_cr;
        boost::shared_ptr<low::anything_to_utf16> first;
    };

    typedef base_csink<char> csink;

    typedef boost::iostreams::stream_buffer<csink> ocstream_buffer;
    typedef boost::iostreams::stream<csink> ocstream;

#ifdef BOOST_CSTREAM_USE_WCHAR_T
    typedef base_csink<wchar_t> wcsink;

    typedef boost::iostreams::stream_buffer<wcsink> wocstream_buffer;
    typedef boost::iostreams::stream<wcsink> wocstream;

#endif
}
}

#endif // BOOST_CSTREAM_CSINK_HPP
