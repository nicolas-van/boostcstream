
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_LOW_DEBUG_UTILS_HPP
#define BOOST_CSTREAM_LOW_DEBUG_UTILS_HPP

#if !defined(BOOST_CSTREAM_LOW_DEBUG_UTILS_DEBUG_MODE)
#if defined(NDEBUG)
#define BOOST_CSTREAM_LOW_DEBUG_UTILS_DEBUG_MODE 0
#else
#define BOOST_CSTREAM_LOW_DEBUG_UTILS_DEBUG_MODE 1
#endif
#endif

#if BOOST_CSTREAM_LOW_DEBUG_UTILS_DEBUG_MODE==0
#define BOOST_CSTREAM_LOW_DEBUG_UTILS_ACTION(x)	((void)0)
#else
#define BOOST_CSTREAM_LOW_DEBUG_UTILS_ACTION(x) (x)
#endif

#include <stdexcept>
#include <iostream>

namespace boost {
namespace cstream {
namespace low {
    class assertion_error : public std::logic_error {
        std::string _verif;
        std::string _file;
        unsigned int _line;
        public:
        assertion_error(const std::string& st)
        :logic_error(st),_line(0) {}
        assertion_error(const std::string& st
                ,const std::string& v,const std::string& f
                ,unsigned int l)
        :logic_error(st),_verif(v),_file(f),_line(l) {}
        const std::string& verification() const {
            return _verif;
        }
        const std::string& file() const {
            return _file;
        }
        unsigned int line() const {
            return _line;
        }
        ~assertion_error() throw() {}
    };
}
}
}

#define BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(e,y) \
BOOST_CSTREAM_LOW_DEBUG_UTILS_ACTION( (e)?(void)0:( \
throw boost::cstream::low::assertion_error(y,#e,__FILE__, __LINE__)) )

#endif
