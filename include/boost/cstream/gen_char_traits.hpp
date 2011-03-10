
//          Copyright Nicolas Vanhoren 2008 - 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CSTREAM_GEN_CHAR_TRAITS_HPP
#define BOOST_CSTREAM_GEN_CHAR_TRAITS_HPP

#include <iosfwd>
#include <algorithm>

namespace boost {
namespace cstream {

template<typename _CharT>
    struct gen_char_traits
    {
      typedef _CharT                                    char_type;
      typedef unsigned long    int_type;
      typedef std::streampos    pos_type;
      typedef std::streamoff    off_type;
      typedef std::mbstate_t  state_type;

      static void
      assign(char_type& __c1, const char_type& __c2)
      { __c1 = __c2; }

      static bool
      eq(const char_type& __c1, const char_type& __c2)
      { return __c1 == __c2; }

      static bool
      lt(const char_type& __c1, const char_type& __c2)
      { return __c1 < __c2; }

      static int
      compare(const char_type* __s1, const char_type* __s2, std::size_t __n);

      static std::size_t
      length(const char_type* __s);

      static const char_type*
      find(const char_type* __s, std::size_t __n, const char_type& __a);

      static char_type*
      move(char_type* __s1, const char_type* __s2, std::size_t __n);

      static char_type*
      copy(char_type* __s1, const char_type* __s2, std::size_t __n);

      static char_type*
      assign(char_type* __s, std::size_t __n, char_type __a);

      static char_type
      to_char_type(const int_type& __c)
      { return static_cast<char_type>(__c); }

      static int_type
      to_int_type(const char_type& __c)
      { return static_cast<int_type>(__c); }

      static bool
      eq_int_type(const int_type& __c1, const int_type& __c2)
      { return __c1 == __c2; }

      static int_type
      eof()
      { return static_cast<int_type>(EOF); }

      static int_type
      not_eof(const int_type& __c)
      { return !eq_int_type(__c, eof()) ? __c : to_int_type(char_type()); }
    };

  template<typename _CharT>
    int
    gen_char_traits<_CharT>::
    compare(const char_type* __s1, const char_type* __s2, std::size_t __n)
    {
      for (std::size_t __i = 0; __i < __n; ++__i)
	if (lt(__s1[__i], __s2[__i]))
	  return -1;
	else if (lt(__s2[__i], __s1[__i]))
	  return 1;
      return 0;
    }

  template<typename _CharT>
    std::size_t
    gen_char_traits<_CharT>::
    length(const char_type* __p)
    {
      std::size_t __i = 0;
      while (!eq(__p[__i], char_type()))
        ++__i;
      return __i;
    }

  template<typename _CharT>
    const typename gen_char_traits<_CharT>::char_type*
    gen_char_traits<_CharT>::
    find(const char_type* __s, std::size_t __n, const char_type& __a)
    {
      for (std::size_t __i = 0; __i < __n; ++__i)
        if (eq(__s[__i], __a))
          return __s + __i;
      return 0;
    }

  template<typename _CharT>
    typename gen_char_traits<_CharT>::char_type*
    gen_char_traits<_CharT>::
    move(char_type* __s1, const char_type* __s2, std::size_t __n)
    {
      return static_cast<_CharT*>(std::memmove(__s1, __s2,
					       __n * sizeof(char_type)));
    }

  template<typename _CharT>
    typename gen_char_traits<_CharT>::char_type*
    gen_char_traits<_CharT>::
    copy(char_type* __s1, const char_type* __s2, std::size_t __n)
    {
      std::copy(__s2, __s2 + __n, __s1);
      return __s1;
    }

  template<typename _CharT>
    typename gen_char_traits<_CharT>::char_type*
    gen_char_traits<_CharT>::
    assign(char_type* __s, std::size_t __n, char_type __a)
    {
      std::fill_n(__s, __n, __a);
      return __s;
    }

}
}

#endif // BOOST_CSTREAM_GEN_CHAR_TRAITS_HPP
