
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_FCSTREAM_HPP
#define BOOST_CSTREAM_FCSTREAM_HPP

#include "cstream.hpp"
#include <fstream>

namespace boost {
namespace cstream {

    /**
        Convenience specilisation of base_csink to convert text and
        directly store it in a file.
    */
    template <typename CharType
            ,typename Trait=encoding_traits<CharType> >
    struct base_fcsink {

        typedef CharType char_type;
        struct category : boost::iostreams::sink_tag
                ,boost::iostreams::flushable_tag
                ,boost::iostreams::closable_tag {};
        typedef Trait encoding_trait;

        /**
            Constructor specifying the name of the file, the writing
            new line policy and the open mode. The platform default
            character set is assumed.
        */
        base_fcsink(const std::string& filename
                ,writing_new_line_policy nlpol = write_auto
                ,std::ios_base::openmode mode = std::ios_base::out)
                : output(new std::ofstream(filename.c_str()
                        ,mode | std::ios_base::binary))
                , conv_sink(boost::ref(*output),"",nlpol) {}

        /**
            Constructor specifying the name of the file, the character
            set of the file, the writing new line policy and the open
            mode.
        */
        base_fcsink(const std::string& filename
                ,const std::string& destination
                ,writing_new_line_policy nlpol = write_auto
                ,std::ios_base::openmode mode = std::ios_base::out)
                : output(new std::ofstream(filename.c_str()
                        ,mode | std::ios_base::binary))
                , conv_sink(boost::ref(*output),destination,nlpol) {}

        std::streamsize write(const char_type* s,std::streamsize n) {
            return conv_sink.write(s,n);
        }

        bool flush() {
            return conv_sink.flush();
        }

        void close() {
            output->close();
        }

        private:
        boost::shared_ptr<std::ofstream> output;
        base_csink<CharType,Trait> conv_sink;
    };

    /**
        Convenience specialisation of base_csource to read a file and
        convert it at the same time.
    */
    template <typename CharType
            ,typename Trait=encoding_traits<CharType> >
    struct base_fcsource {

        typedef CharType char_type;
        struct category : boost::iostreams::source_tag
                ,boost::iostreams::closable_tag {};
        typedef Trait encoding_trait;

        /**
            Constructor specifying the name of the file,
            the reading new line policy and the open mode.
            The platform default character set is assumed.
        */
        base_fcsource(const std::string& filename
                ,reading_new_line_policy nlpol = read_auto
                ,std::ios_base::openmode mode = std::ios_base::in)
                : input(new std::ifstream(filename.c_str()
                        ,mode | std::ios_base::binary))
                , conv_source(boost::ref(*input),"",nlpol) {}

        /**
            Constructor specifying the name of the file, the character
            set of the file, the reading new line policy and the open
            mode.
        */
        base_fcsource(const std::string& filename
                ,const std::string& origin
                ,reading_new_line_policy nlpol = read_auto
                ,std::ios_base::openmode mode = std::ios_base::in)
                : input(new std::ifstream(filename.c_str()
                        ,mode | std::ios_base::binary))
                , conv_source(boost::ref(*input),origin,nlpol) {}

        std::streamsize read(char_type* s,std::streamsize n) {
            return conv_source.read(s,n);
        }

        void close() {
            input->close();
        }

        private:
        boost::shared_ptr<std::ifstream> input;
        base_csource<CharType,Trait> conv_source;
    };

    typedef base_fcsink<char> fcsink;
    typedef base_fcsource<char> fcsource;

    typedef boost::iostreams::stream_buffer<fcsink > ofcstream_buffer;
    typedef boost::iostreams::stream_buffer<fcsource > ifcstream_buffer;

    typedef boost::iostreams::stream<fcsink > ofcstream;
    typedef boost::iostreams::stream<fcsource > ifcstream;

#ifdef BOOST_CSTREAM_USE_WCHAR_T
    typedef base_fcsink<wchar_t> wfcsink;
    typedef base_fcsource<wchar_t> wfcsource;

    typedef boost::iostreams::stream_buffer<wfcsink > wofcstream_buffer;
    typedef boost::iostreams::stream_buffer<wfcsource > wifcstream_buffer;

    typedef boost::iostreams::stream<wfcsink > wofcstream;
    typedef boost::iostreams::stream<wfcsource > wifcstream;
#endif

}
}

#endif // BOOST_CSTREAM_FCSTREAM_HPP
