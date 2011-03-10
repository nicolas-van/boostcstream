
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "low_icu.hpp"
#include <unicode/ucnv.h>
#include <stdexcept>
#include <vector>
#include <limits>
#include "debug_utils.hpp"
#include "endian.hpp"

using namespace std;
using namespace boost::cstream;

namespace boost {
namespace cstream {
namespace low {

    const float utf16_4bytes_prob = 0.1;
    const float buffer_overflow_grow = 0.1;
    const float end_line_prob = 0.1;

    converter_base::converter_base(const std::string& charset) {
        UErrorCode err = U_ZERO_ERROR;
        real = ucnv_open(charset.c_str(), &err);
        if(U_FAILURE(err) || real==NULL) {
            if(real!=NULL) {
                ucnv_close(static_cast<UConverter*>(real));
                real=NULL;
            }
            throw conversion_error("could not open converter");
        }
    }

    size_t converter_base::get_min_size() {
        return ucnv_getMinCharSize(static_cast<UConverter*>(real));
    }

    converter_base::~converter_base() {
        if(real!=NULL)
            ucnv_close(static_cast<UConverter*>(real));
    }

#define __input (static_cast<UConverter*>(input.real))
#define __output (static_cast<UConverter*>(output.real))

    template<typename A>
    void add_percent(vector<A>& vec,size_t multiple
            ,float percent=buffer_overflow_grow) {
        size_t size = vec.size()+(vec.size()*percent);
        if(size==vec.size())
            size++;
        if(size%multiple!=0)
            size+=multiple-(size%multiple);
        vec.resize(size);
    }

    std::streamsize anything_to_utf16::write(const char* s, std::streamsize n) {
        UErrorCode err = U_ZERO_ERROR;

        const char* source=s;
        const char* sourceend=s+n;

        vector<UChar> uni_buf(static_cast<size_t>(ucnv_getMinCharSize(__input)
                *n*(utf16_4bytes_prob+1)));
        size_t unibegin = 0;

        while(true) {
            UChar* unitarget = &uni_buf[unibegin];

            ucnv_toUnicode(__input, &unitarget, unitarget+(uni_buf.size()-unibegin),
                        &source, sourceend, NULL, false, &err);
            unibegin=unitarget-&uni_buf[0];
            if(err == U_BUFFER_OVERFLOW_ERROR) {
                err = U_ZERO_ERROR;
                add_percent(uni_buf,1);
            } else
                break;
        }
        if(U_FAILURE(err))
            throw conversion_error();

        if(dest->write(reinterpret_cast<char*>(&uni_buf[0])
            ,unibegin*sizeof(UChar))==0) {
            return 0;
        }
        return n;
    }

    std::streamsize utf16_to_anything::write(const char* s, std::streamsize n) {
        BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT((n%sizeof(UChar)==0)
            ,"not a utf16 input in utf16_to_anything");
        UErrorCode err = U_ZERO_ERROR;

        const UChar* unisource = reinterpret_cast<const UChar*>(s);
        const UChar* unisourceend = reinterpret_cast<const UChar*>(s+n);
        vector<char> target_buf(ucnv_getMaxCharSize(__output)*(unisourceend-unisource));
        size_t begin=0;

        while(true) {
            char* target=&target_buf[begin];
            char* targetend=target+(target_buf.size()-begin);
            ucnv_fromUnicode(__output, &target, targetend ,
                &unisource, unisourceend, NULL, false, &err);
            begin=target-&target_buf[0];
            if(err == U_BUFFER_OVERFLOW_ERROR) {
                err = U_ZERO_ERROR;
                add_percent(target_buf,1);
            } else
                break;
        }
        if(U_FAILURE(err))
            throw conversion_error();

        std::streamsize size=begin;

        if(dest->write(&target_buf[0],size)==0) {
            return 0;
        }
        return n;
    }

    std::streamsize utf16_from_anything::read(char* s, std::streamsize n) {
        BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(n%sizeof(UChar)==0,
            "not a utf16 output in utf16_from_anything");
        UErrorCode err = U_ZERO_ERROR;

        UChar* target = reinterpret_cast<UChar*>(s);
        UChar* targetend = reinterpret_cast<UChar*>(s+n);

        while(tmp_begin<tmp_end && target<targetend) {
            (*target) = *reinterpret_cast<UChar*>(&temporary[tmp_begin]);
            tmp_begin+=sizeof(UChar);
            target++;
        }
        if(tmp_begin>=tmp_end)
            temporary.resize(0);

        while(true) {
            vector<char> buffer(ucnv_getMaxCharSize(__input)*(targetend-target));
            std::streamsize asked = buffer.size();
            std::streamsize readed = from->read(&buffer[0],asked);

            const char* source = &buffer[0];
            const char* sourceend = &buffer[0]+readed;

            ucnv_toUnicode(__input, &target, targetend,
                &source, sourceend, NULL, false, &err);

            if(target==targetend) {
                if(err == U_BUFFER_OVERFLOW_ERROR) {
                    err = U_ZERO_ERROR;
                    //put it in a buffer then return n
                    temporary.resize((sourceend-source)
                        *ucnv_getMinCharSize(__input)*sizeof(UChar));
                    tmp_begin=0;
                    tmp_end=0;

                    while(true) {
                        UChar* buftarget = reinterpret_cast<UChar*>(&temporary[tmp_end]);
                        UChar* buftargetend = buftarget
                            +((temporary.size()-tmp_end)/sizeof(UChar));

                        ucnv_toUnicode(__input, &buftarget, buftargetend,
                            &source, sourceend, NULL, false, &err);
                        tmp_end=reinterpret_cast<char*>(buftarget)-&temporary[0];
                        if(err==U_BUFFER_OVERFLOW_ERROR) {
                            err = U_ZERO_ERROR;
                            add_percent(temporary,sizeof(UChar));
                        } else {
                            if(U_FAILURE(err))
                                throw conversion_error();
                            break;
                        }
                    }
                    return n;
                } else {
                    if(U_FAILURE(err))
                        throw conversion_error();
                    // readed data sise == unicode size
                    return n;
                }
            } else {
                if(U_FAILURE(err))
                    throw conversion_error();
                BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(source==sourceend,"ICU does not work "
                    "the way I was thinking");
                if(asked>readed)
                    // no more chars
                    return reinterpret_cast<char*>(target)-s;

                //need more data
            }
        }
    }

    std::streamsize anything_from_utf16::read(char* s, std::streamsize n) {
        UErrorCode err = U_ZERO_ERROR;

        char* target = s;
        char* targetend = s+n;

        while(tmp_begin<tmp_end && target<targetend) {
            *target = temporary[tmp_begin];
            tmp_begin+=1;
            target++;
        }
        if(tmp_begin>=tmp_end)
            temporary.resize(0);

        while(true) {
            vector<UChar> buffer(static_cast<size_t>(ucnv_getMinCharSize(__output)
                    *(targetend-target)*(utf16_4bytes_prob+1)));
            std::streamsize asked = buffer.size();
            std::streamsize readed = from->read(
                reinterpret_cast<char*>(&buffer[0]),asked*sizeof(UChar));
            BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(readed%sizeof(UChar)==0,
                "not a UTF-16 input in anything_from_utf16");
            readed/=sizeof(UChar);

            const UChar* source = &buffer[0];
            const UChar* sourceend = &buffer[0]+readed;

            ucnv_fromUnicode(__output, &target, targetend,
                &source, sourceend, NULL, false, &err);

            if(target==targetend) {
                if(err == U_BUFFER_OVERFLOW_ERROR) {
                    err = U_ZERO_ERROR;
                    //put it in a buffer then return n
                    temporary.resize((sourceend-source)*ucnv_getMaxCharSize(__output));
                    tmp_begin=0;
                    tmp_end=0;

                    while(true) {
                        char* buftarget = &temporary[tmp_end];
                        char* buftargetend = buftarget
                            +(temporary.size()-tmp_end);

                        ucnv_fromUnicode(__output, &buftarget, buftargetend,
                            &source, sourceend, NULL, false, &err);
                        tmp_end=buftarget-&temporary[0];
                        if(err==U_BUFFER_OVERFLOW_ERROR) {
                            err = U_ZERO_ERROR;
                            add_percent(temporary,1);
                        } else {
                            if(U_FAILURE(err))
                                throw conversion_error();
                            break;
                        }
                    }
                    return n;
                } else {
                    if(U_FAILURE(err))
                        throw conversion_error();
                    // readed data sise == final size
                    return n;
                }
            } else {
                if(U_FAILURE(err))
                    throw conversion_error();
                BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(source==sourceend,"ICU does not work "
                    "the way I was thinking");
                if(asked>readed)
                    // no more chars
                    return target-s;

                //need more data
            }
        }
    }

    const UChar cr_val = 0x0D;
    const UChar lf_val = 0x0A;

    std::streamsize crlf_remover::read(char* s, std::streamsize n) {
        BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(n%sizeof(UChar)==0,
            "not utf16 output in crlf_remover");

        UChar* currentpos=reinterpret_cast<UChar*>(s);
        UChar* targetfinal=currentpos+(n/sizeof(UChar));

        while(currentpos<targetfinal) {

            streamsize asked = (targetfinal-currentpos)*sizeof(UChar);
            streamsize readed = from->read(reinterpret_cast<char*>(currentpos),asked);
            BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(readed%sizeof(UChar)==0
                ,"not utf16 input in crlf_remover");
            UChar* target=currentpos;
            UChar* targetend=target+(readed/sizeof(UChar));

            while(target<targetend) {
                *currentpos = *target;
                if(*currentpos==cr_val) {
                    *currentpos=lf_val;
                    lastcr=true;
                } else if (*currentpos==lf_val && lastcr) {
                    currentpos--;
                    lastcr=false;
                } else {
                    lastcr=false;
                }
                target++;
                currentpos++;
            }
            if(readed<asked)
                return reinterpret_cast<char*>(currentpos)-s;
        }

        return n;
    }

    std::streamsize lf_to_crlf_converter::write(const char* s, std::streamsize n) {
        BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(n%sizeof(UChar)==0,
            "not utf16 input in lf_to_crlf_converter");

        const UChar* currentpos=reinterpret_cast<const UChar*>(s);
        const UChar* targetfinal=currentpos+(n/sizeof(UChar));

        vector<UChar> buffer(0,n/sizeof(UChar));
        while(currentpos!=targetfinal) {
            if(*currentpos==lf_val) {
                buffer.push_back(cr_val);
                buffer.push_back(lf_val);
            } else
                buffer.push_back(*currentpos);
            currentpos++;
        }
        std::streamsize writed = to->write(reinterpret_cast<const char*>(&buffer[0])
                ,buffer.size()*sizeof(UChar));
        if(writed == 0)
            return 0;
        return n;
    }

    std::streamsize lf_to_cr_converter::write(const char* s, std::streamsize n) {
        BOOST_CSTREAM_LOW_DEBUG_UTILS_ASSERT(n%sizeof(UChar)==0,
            "not utf16 input in lf_to_cr_converter");

        const UChar* currentpos=reinterpret_cast<const UChar*>(s);
        const UChar* targetfinal=currentpos+(n/sizeof(UChar));
        size_t i=0;

        vector<UChar> buffer(n/sizeof(UChar));
        while(currentpos!=targetfinal) {
            if(*currentpos==lf_val) {
                buffer[i] = cr_val;
            } else
                buffer[i] = *currentpos;
            currentpos++;
            i++;
        }
        std::streamsize writed = to->write(reinterpret_cast<const char*>(&buffer[0])
                ,buffer.size()*sizeof(UChar));
        return writed;
    }

    size_t get_ICU_char_size() {
        return sizeof(UChar);
    }

    std::string get_char_encoding() {
        const char* tmp = ucnv_getDefaultName();
        if(tmp==NULL)
            throw platform_error("unknown char encoding");
        return tmp;
    }

#ifdef BOOST_CSTREAM_USE_WCHAR_T
    std::string get_wchar_encoding() {
        // 100% temporary solution
        if(sizeof(wchar_t)==2) {
            if(is_2_bytes_little_endian())
                return "UTF16LE";
            else
                return "UTF16BE";

        } else if(sizeof(wchar_t)==4) {
            if(is_4_bytes_little_endian())
                return "UTF32LE";
            else
                return "UTF32BE";
        }
        throw platform_error("unknown wchar_t encoding");
    }
#endif

    bool equals_charset(const std::string& one,const std::string& two) {
        return ucnv_compareNames(one.c_str(),two.c_str())==0?true:false;
    }

    boost::cstream::writing_new_line_policy get_new_line_policy() {
#if defined(__CYGWIN__) \
|| defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
        return write_crlf;
#elif (defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)) \
    && !(__MACH__ && !defined(_MSL_USING_MSL_C))
        return write_cr;
#else
        return write_lf;
#endif
    }

}
}
}

