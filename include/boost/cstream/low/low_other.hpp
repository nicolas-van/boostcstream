
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_LOW_LOW_OTHER_HPP
#define BOOST_CSTREAM_LOW_LOW_OTHER_HPP

#include "general.hpp"
#include "../details.hpp"
#include "debug_utils.hpp"
#include <boost/ref.hpp>

namespace boost {
namespace cstream {
namespace low {

    template <typename OStream>
    struct sink_adapter : sink_interface {
        typedef typename OStream::char_type char_type;
        typedef typename OStream::traits_type traits_type;
        typedef std::basic_ostream<char_type,traits_type> stream_type;
        typedef std::basic_streambuf<char_type,traits_type> buffer_type;

        sink_adapter() : stream(NULL) {}

        sink_adapter(stream_type* strea)
                : redirect(), stream(*strea) {}

        sink_adapter(buffer_type* buf)
                : redirect(new stream_type(buf)), stream(*redirect) {}

        bool flush() {stream.get() << std::flush;
            return true;};

        std::streamsize write(const char* s, std::streamsize n) {
            BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(n%sizeof(char_type)==0
                    ,"not correct input in sink_adapter");
            const char_type* alias = reinterpret_cast<const char_type*>(s);
            std::streamsize wri = stream.get().rdbuf()->sputn(alias,n/sizeof(char_type));
            return wri*sizeof(char_type);
        }

        private:
        std::auto_ptr<stream_type> redirect;
        boost::reference_wrapper<stream_type> stream;
    };

    template <typename OStream>
    sink_adapter<OStream>* make_sink(boost::reference_wrapper<OStream> src) {
        return new sink_adapter<OStream>(&src.get());
    }

    template <typename IStream>
    struct source_adapter : source_interface {
        typedef typename IStream::char_type char_type;
        typedef typename IStream::traits_type traits_type;
        typedef std::basic_istream<char_type,traits_type> stream_type;
        typedef std::basic_streambuf<char_type,traits_type> buffer_type;

        source_adapter() : stream(NULL) {}

        source_adapter(stream_type* strea)
                : redirect(), stream(*strea) {}

        source_adapter(buffer_type* buf)
                : redirect(new stream_type(buf)), stream(*redirect) {}

        std::streamsize read(char* s, std::streamsize n) {
            BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(n%sizeof(char_type)==0
                    ,"not correct output in source_adapter");
            char_type* alias = reinterpret_cast<char_type*>(s);
            std::streamsize rea = stream.get().rdbuf()->sgetn(alias,n/sizeof(char_type));
            return rea*sizeof(char_type);
        }

        private:
        std::auto_ptr<stream_type> redirect;
        boost::reference_wrapper<stream_type> stream;
    };

    template <typename IStream>
    source_adapter<IStream>* make_source(boost::reference_wrapper<IStream> src) {
        return new source_adapter<IStream>(&src.get());
    }

}
}
}

#endif // BOOST_CSTREAM_LOW_LOW_OTHER_HPP
