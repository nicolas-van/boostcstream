
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOSTR_CSTREAM_CONVERT_HPP
#define BOOSTR_CSTREAM_CONVERT_HPP

#include <sstream>
#include "cstream.hpp"

namespace boost {
namespace cstream {

    template<typename Dst,typename SrcChar,typename SrcTrait>
    Dst convert_string(const std::basic_string<SrcChar,SrcTrait>& src) {
        typedef SrcChar src_char_type;
        typedef SrcTrait src_char_trait;
        typedef typename Dst::value_type dst_char_type;
        typedef typename Dst::traits_type dst_char_trait;
        typedef encoding_traits<src_char_type,src_char_trait> src_enc;
        typedef base_csink<src_char_type,src_enc> sink_type;

        std::basic_stringstream<dst_char_type,dst_char_trait> res;
        boost::iostreams::stream<sink_type> cres(boost::ref(res));
        cres << src << std::flush;
        return res.str();
    }

}
}

#endif // BOOSTR_CSTREAM_CONVERT_HPP
