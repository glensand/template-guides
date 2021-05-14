/* Copyright (C) 2021 Gleb Bezborodov - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * You should have received a copy of the MIT license with
 * this file. If not, please write to: bezborodoff.gleb@gmail.com, or visit : https://github.com/glensand/hope
 */

#include "gtest/gtest.h"

#include "hope/tuple/flat_tuple.h"
#include "hope/tuple/tuple_from_struct.h"

namespace detail {
    struct test_struct final {
        int _0;
        float _1;
        double _2;
    };
}

TEST(StructToRefTuple, AllCvModifiedTypesAreEqual)
{
    // for some undefined reason gcc compiler does not support ref qualifiers at the constexpr context  
    constexpr detail::test_struct instance{ 1, 1.1f, 1.3 };

#if _MSC_VER && !__INTEL_COMPILER
    constexpr auto ref = hope::tuple_from_struct(instance, hope::field_policy::reference{});
#else
    auto ref = hope::tuple_from_struct(instance, hope::field_policy::reference{});
#endif

#undef HOPE_ASSERT
#if _MSC_VER && !__INTEL_COMPILER
#   define HOPE_ASSERT(EXPR) static_assert(EXPR)
#else
#   define HOPE_ASSERT(EXPR) ASSERT_TRUE(EXPR)
#endif

    HOPE_ASSERT(ref.get<0>() == ref.get<int>());
    HOPE_ASSERT(ref.get<0>() == ref.get<int&>());
    HOPE_ASSERT(ref.get<0>() == ref.get<const int&>());
    HOPE_ASSERT(ref.get<0>() == ref.get<const int>());

    HOPE_ASSERT(ref.get<1>() == ref.get<float>());
    HOPE_ASSERT(ref.get<1>() == ref.get<float&>());
    HOPE_ASSERT(ref.get<1>() == ref.get<const float&>());
    HOPE_ASSERT(ref.get<1>() == ref.get<const float>());
}

TEST(StructToRefTuple, StructureCanBeModifiedViaTuple)
{
    detail::test_struct instance{ 1, 1.1f, 1.3 };
    auto&& ref = hope::tuple_from_struct(instance, hope::field_policy::reference{});
    ref.get<0>() = 42;
    ref.get<1>() = 42.f;

    ASSERT_TRUE(42 == instance._0);
    ASSERT_TRUE(abs(42.f - instance._1) < FLT_EPSILON);
}
