///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt
//

#include <boost/detail/lightweight_test.hpp>
#include <boost/math/special_functions/sign.hpp>

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#if !defined(TEST_CPP_BIN_FLOAT) && !defined(TEST_DOUBLE) && !defined(TEST_MPFR_FLOAT)
#define TEST_CPP_BIN_FLOAT
#define TEST_DOUBLE
#define TEST_MPFR_FLOAT
#endif


#ifdef TEST_CPP_BIN_FLOAT
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_complex.hpp>
#endif
#ifdef TEST_MPFR_FLOAT
#include <boost/multiprecision/mpfr.hpp>
#endif

template <class T>
struct extract_value_type
{
   typedef typename T::value_type type;
};
template <>
struct extract_value_type<double>
{
   typedef double type;
};

template <class T>
typename std::enable_if<std::is_same<T, typename extract_value_type<T>::type>::value, bool>::type get_sign_bit(const T& val)
{
   return boost::math::signbit(val);
}
template <class T>
typename std::enable_if<!std::is_same<T, typename extract_value_type<T>::type>::value, bool>::type get_sign_bit(const T& val)
{
   return boost::math::signbit(real(val));
}


template <class T>
void test()
{
   using value_type = typename extract_value_type<T>::type;

   using std::real;

   T mp_zero{0};
   T mp_m_zero{-real(mp_zero)};
   T mp_finite{2};
   T mp_m_finite{-2};
   T mp_big{!std::is_same<T, value_type>::value ? std::numeric_limits<value_type>::has_infinity ? std::numeric_limits<value_type>::infinity() : (std::numeric_limits<value_type>::max)() : std::numeric_limits<T>::has_infinity ? std::numeric_limits<T>::infinity()
                                                                                                                                                                                                     : (std::numeric_limits<T>::max)()};
   T mp_m_big{-real(mp_big)};
   T mp_small{!std::is_same<T, value_type>::value ? (std::numeric_limits<value_type>::min)() : (std::numeric_limits<T>::min)()};
   T mp_m_small{-real(mp_small)};

   T result;
   //
   // Multiplications:
   //
   result = mp_zero * mp_finite;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_zero * mp_m_finite;
   BOOST_TEST(get_sign_bit(result));
   result = mp_m_zero * mp_finite;
   BOOST_TEST(get_sign_bit(result));
   result = mp_m_zero * mp_m_finite;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_zero * -real(mp_finite);
   BOOST_TEST(get_sign_bit(result));
   result = -real(mp_zero) * mp_finite;
   BOOST_TEST(get_sign_bit(result));
   result = -real(mp_zero) * -real(mp_finite);
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_small * mp_small;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_small * mp_m_small;
   BOOST_TEST(get_sign_bit(result));
   result = mp_m_small * mp_m_small;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_small * -real(mp_small);
   BOOST_TEST(get_sign_bit(result));
   result = -real(mp_small) * -real(mp_small);
   BOOST_TEST(get_sign_bit(result) == 0);
   //
   // Divisions:
   //
   result = mp_zero / mp_finite;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_zero / mp_m_finite;
   if constexpr (std::is_same<value_type, T>::value)
   {
      BOOST_TEST(get_sign_bit(result));
   }
   else
   {
      // Complex result is slightly different:
      BOOST_TEST(!get_sign_bit(real(result)));
      BOOST_TEST(get_sign_bit(imag(result)));
   }
   result = mp_m_zero / mp_finite;
   if constexpr (std::is_same<value_type, T>::value)
   {
      BOOST_TEST(get_sign_bit(result));
   }
   else
   {
      // Complex result is slightly different:
      BOOST_TEST(!get_sign_bit(real(result)));
      BOOST_TEST(!get_sign_bit(imag(result)));
   }
   result = mp_m_zero / mp_m_finite;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_zero / -mp_finite;
   BOOST_TEST(get_sign_bit(result));
   result = -mp_zero / mp_finite;
   BOOST_TEST(get_sign_bit(result));
   result = -mp_zero / -mp_finite;
   BOOST_TEST(get_sign_bit(result) == 0);

   result = mp_small / mp_big;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_small / mp_m_big;
   if constexpr (std::is_same<value_type, T>::value)
   {
      BOOST_TEST(get_sign_bit(result));
   }
   else
   {
      // Complex result is slightly different:
      BOOST_TEST(!get_sign_bit(real(result)));
      BOOST_TEST(get_sign_bit(imag(result)));
   }
   result = mp_m_small / mp_big;
   if constexpr (std::is_same<value_type, T>::value)
   {
      BOOST_TEST(get_sign_bit(result));
   }
   else
   {
      // Complex result is slightly different:
      BOOST_TEST(!get_sign_bit(real(result)));
      BOOST_TEST(!get_sign_bit(imag(result)));
   }
   result = mp_m_small / mp_m_big;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_small / -mp_big;
   BOOST_TEST(get_sign_bit(result));
   result = -mp_small / mp_big;
   BOOST_TEST(get_sign_bit(result));
   result = -mp_small / -mp_big;
   BOOST_TEST(get_sign_bit(result) == 0);
   //
   // Additions:
   //
   result = mp_zero + mp_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_zero + mp_m_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_m_zero + mp_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_m_zero + mp_m_zero;
   BOOST_TEST(get_sign_bit(result));
   //
   // Subtractions:
   //
   result = mp_zero - mp_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_zero - mp_m_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_m_zero - mp_zero;
   BOOST_TEST(get_sign_bit(result));
   result = mp_m_zero - mp_m_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_finite - mp_finite;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_finite + mp_m_finite;
   BOOST_TEST(get_sign_bit(result) == 0);

   if constexpr (std::is_same<T, value_type>::value)
   {
      //
      // Over again for one arg an integer:
      //
      int i_zero{0};
      int i_finite{2};
      int i_m_finite{-2};
      result = i_zero * mp_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = i_zero * mp_m_finite;
      BOOST_TEST(get_sign_bit(result));
      result = i_zero * -mp_finite;
      BOOST_TEST(get_sign_bit(result));
      result = mp_zero * i_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_m_zero * i_finite;
      BOOST_TEST(get_sign_bit(result));
      result = mp_zero * i_m_finite;
      BOOST_TEST(get_sign_bit(result));
      //
      // Divisions:
      //
      result = i_zero / mp_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = i_zero / mp_m_finite;
      BOOST_TEST(get_sign_bit(result));
      result = mp_zero / i_m_finite;
      BOOST_TEST(get_sign_bit(result));
      result = -mp_zero / i_finite;
      BOOST_TEST(get_sign_bit(result));
      //
      // Additions:
      //
      result = mp_zero + i_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
#ifndef TEST_MPFR_FLOAT
      //
      // There appears to be a bug in mpfr_add_ui here:
      //
      result = i_zero + mp_m_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
#endif
      //
      // Subtractions:
      //
      result = mp_zero - i_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = i_zero - mp_m_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_m_zero - i_zero;
      BOOST_TEST(get_sign_bit(result));
      result = i_finite - mp_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = i_finite + mp_m_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_finite - i_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_finite + i_m_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      //
      // Over again for one arg a float:
      //
      float f_zero{0};
      float f_m_zero{-f_zero};
      float f_finite{2};
      float f_m_finite{-2};
      result = f_zero * mp_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_zero * mp_m_finite;
      BOOST_TEST(get_sign_bit(result));
      result = f_zero * -mp_finite;
      BOOST_TEST(get_sign_bit(result));
      result = mp_zero * f_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_m_zero * f_finite;
      BOOST_TEST(get_sign_bit(result));
      result = mp_zero * f_m_finite;
      BOOST_TEST(get_sign_bit(result));
      //
      // Divisions:
      //
      result = f_zero / mp_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_zero / mp_m_finite;
      BOOST_TEST(get_sign_bit(result));
      result = mp_zero / f_m_finite;
      BOOST_TEST(get_sign_bit(result));
      result = -mp_zero / f_finite;
      BOOST_TEST(get_sign_bit(result));
      //
      // Additions:
      //
      result = f_zero + mp_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_zero + mp_m_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_m_zero + mp_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_m_zero + mp_m_zero;
      BOOST_TEST(get_sign_bit(result));
      //
      // Subtractions:
      //
      result = f_zero - mp_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_zero - mp_m_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_m_zero - mp_zero;
      BOOST_TEST(get_sign_bit(result));
      result = f_m_zero - mp_m_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_zero - f_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_zero - f_m_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_m_zero - f_zero;
      BOOST_TEST(get_sign_bit(result));
      result = mp_m_zero - f_m_zero;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_finite - mp_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = f_finite + mp_m_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_finite - f_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
      result = mp_finite + f_m_finite;
      BOOST_TEST(get_sign_bit(result) == 0);
   }
   //
   // Special cases:
   //
   result = mp_m_zero * mp_m_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = mp_m_zero + mp_m_zero;
   BOOST_TEST(get_sign_bit(result));
   result = mp_zero + mp_zero;
   BOOST_TEST(get_sign_bit(result) == 0);
   result = abs(mp_m_zero);
   BOOST_TEST(get_sign_bit(result) == 0);
   if constexpr (std::is_same<T, value_type>::value)
   {
      result = fabs(mp_m_zero);
      BOOST_TEST(get_sign_bit(result) == 0);
   }
   result = sqrt(mp_m_zero);
   BOOST_TEST(get_sign_bit(result));
}

int main()
{
#ifdef TEST_DOUBLE
   test<double>();
#endif
#ifdef TEST_CPP_BIN_FLOAT
   test<boost::multiprecision::cpp_bin_float_50>();
   test<boost::multiprecision::cpp_complex_50>();
   test<boost::multiprecision::number<boost::multiprecision::cpp_bin_float<35, boost::multiprecision::digit_base_10, std::allocator<char>, boost::long_long_type>, boost::multiprecision::et_on>>();
#endif
#ifdef TEST_MPFR_FLOAT
   test<boost::multiprecision::mpfr_float_50>();
#endif
   return boost::report_errors();
}
