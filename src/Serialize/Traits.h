#ifndef THORS_ANVIL_SERIALIZE_TRAITS_H
#define THORS_ANVIL_SERIALIZE_TRAITS_H
/*
 *  This file is all about defineing the template type Traits<T>
 *
 *  Two macros are provides to facilitate the building of Traits<T>
 *  specializations for user defined types.
 *
 *      ThorsAnvil_MakeTraitCustom(DataType)    // Will use operator << and operator >>
 *      ThorsAnvil_PointerAllocator(DataType, Action)
 *      ThorsAnvil_MakeTrait(DataType, ...)
 *      ThorsAnvil_ExpandTrait(ParentType, DataType, ...)
 *      ThorsAnvil_Template_MakeTrait(TemplateParameterCount, DataType, ...)
 *      ThorsAnvil_Template_ExpandTrait(TemplateParameterCount, ParentType, DataType, ...)
 *      ThorsAnvil_MakeEnum(<EnumType>, <EnumValues>...)
 *
 *      ThorsAnvil_PolyMorphicSerializer(Type)
 *      ThorsAnvil_RegisterPolyMorphicType(Type)
 *
 * See README.md for examples.
 */

#include "ThorsSerializerUtil.h"
#include <string>
#include <tuple>
#include <map>
#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include <cstddef>

/*
 * Macros for counting the number of arguments
 * Currently set up for a max of 20.
 */
#define NUM_ARGS(...)          NUM_ARGS_(0, __VA_ARGS__, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 00, Ignore)
#define NUM_ARGS_(Zero, One, I1, I2, I3, I4 ,I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16, I17, I18, I19, I20, I21, I22, I23, I24, I25, I26, I27, I28, I29, I30, I31, I32, I33, I34, I35, I36, I37, I38, I39, I40, I41, I42, I43, A, ...)  A

/*
 * Macros to quote the parameter
 * Used below by the actions.
 */
#define QUOTE_(A)                       A
#define QUOTE(A)                        QUOTE_(#A)

/*
 * Macro to force expansion of a macro that takes two parameters.
 * Used by REP_*
 */
#define EXPAND_(Result)                 Result
#define EXPAND(Act, TC, P1, P2)         EXPAND_(Act(TC, P1, P2))
#define ALT_EXPAND_(Result)             Result
#define ALT_EXPAND(Act, Ex, Id)         EXPAND_(Act(Ex, Id))

/*
 * Macros that that applies the action `Act` (a two parameter macro)
 * To P1 (first argument)
 * and a list of second arguments (placing a comma between each macro).
 *
 * Because NUM_ARGS is limited to 20, This expansion is also limited to 20
 */
#define REP_N(Act, TC, P1, ...)             REP_OF_N(Act, TC, P1, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)
#define REP_OF_N(Act, TC, P1, Count, ...)   REP_OF_N_(Act, TC, P1, Count, __VA_ARGS__)
#define REP_OF_N_(Act, TC, P1, Count, ...)  REP_OF_ ## Count(Act, TC, P1, __VA_ARGS__)

#define REP_OF_43(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_42(Act, TC, P1, __VA_ARGS__)
#define REP_OF_42(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_41(Act, TC, P1, __VA_ARGS__)
#define REP_OF_41(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_40(Act, TC, P1, __VA_ARGS__)
#define REP_OF_40(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_39(Act, TC, P1, __VA_ARGS__)
#define REP_OF_39(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_38(Act, TC, P1, __VA_ARGS__)
#define REP_OF_38(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_37(Act, TC, P1, __VA_ARGS__)
#define REP_OF_37(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_36(Act, TC, P1, __VA_ARGS__)
#define REP_OF_36(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_35(Act, TC, P1, __VA_ARGS__)
#define REP_OF_35(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_34(Act, TC, P1, __VA_ARGS__)
#define REP_OF_34(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_33(Act, TC, P1, __VA_ARGS__)
#define REP_OF_33(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_32(Act, TC, P1, __VA_ARGS__)
#define REP_OF_32(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_31(Act, TC, P1, __VA_ARGS__)
#define REP_OF_31(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_30(Act, TC, P1, __VA_ARGS__)
#define REP_OF_30(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_29(Act, TC, P1, __VA_ARGS__)
#define REP_OF_29(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_28(Act, TC, P1, __VA_ARGS__)
#define REP_OF_28(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_27(Act, TC, P1, __VA_ARGS__)
#define REP_OF_27(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_26(Act, TC, P1, __VA_ARGS__)
#define REP_OF_26(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_25(Act, TC, P1, __VA_ARGS__)
#define REP_OF_25(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_24(Act, TC, P1, __VA_ARGS__)
#define REP_OF_24(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_23(Act, TC, P1, __VA_ARGS__)
#define REP_OF_23(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_22(Act, TC, P1, __VA_ARGS__)
#define REP_OF_22(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_21(Act, TC, P1, __VA_ARGS__)
#define REP_OF_21(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_20(Act, TC, P1, __VA_ARGS__)
#define REP_OF_20(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_19(Act, TC, P1, __VA_ARGS__)
#define REP_OF_19(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_18(Act, TC, P1, __VA_ARGS__)
#define REP_OF_18(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_17(Act, TC, P1, __VA_ARGS__)
#define REP_OF_17(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_16(Act, TC, P1, __VA_ARGS__)
#define REP_OF_16(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_15(Act, TC, P1, __VA_ARGS__)
#define REP_OF_15(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_14(Act, TC, P1, __VA_ARGS__)
#define REP_OF_14(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_13(Act, TC, P1, __VA_ARGS__)
#define REP_OF_13(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_12(Act, TC, P1, __VA_ARGS__)
#define REP_OF_12(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_11(Act, TC, P1, __VA_ARGS__)
#define REP_OF_11(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_10(Act, TC, P1, __VA_ARGS__)
#define REP_OF_10(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_9(Act, TC, P1, __VA_ARGS__)
#define REP_OF_9(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_8(Act, TC, P1, __VA_ARGS__)
#define REP_OF_8(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_7(Act, TC, P1, __VA_ARGS__)
#define REP_OF_7(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_6(Act, TC, P1, __VA_ARGS__)
#define REP_OF_6(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_5(Act, TC, P1, __VA_ARGS__)
#define REP_OF_5(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_4(Act, TC, P1, __VA_ARGS__)
#define REP_OF_4(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_3(Act, TC, P1, __VA_ARGS__)
#define REP_OF_3(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1 ,P2), REP_OF_2(Act, TC, P1, __VA_ARGS__)
#define REP_OF_2(Act, TC, P1, P2, ...)     EXPAND(Act, TC, P1, P2), REP_OF_1(Act, TC, P1, __VA_ARGS__)
#define REP_OF_1(Act, TC, P1, P2, One)     EXPAND(Act, TC, P1, P2)
#define REP_OF_00(Act, TC, P1, One)         LAST_ ## Act(TC, P1)

#define ALT_REP_OF_N(Act, E, P, S, Count)  ALT_REP_OF_N_(Act, E, P, S, Count)
#define ALT_REP_OF_N_(Act, E, P, S, Count) ALT_REP_OF_ ## Count(Act, E, P, S)

#define ALT_REP_OF_43(Act, E, P, S)     P ALT_EXPAND(Act, E, 43), ALT_REP_OF_42(Act, E,  , S)
#define ALT_REP_OF_42(Act, E, P, S)     P ALT_EXPAND(Act, E, 42), ALT_REP_OF_41(Act, E,  , S)
#define ALT_REP_OF_41(Act, E, P, S)     P ALT_EXPAND(Act, E, 41), ALT_REP_OF_40(Act, E,  , S)
#define ALT_REP_OF_40(Act, E, P, S)     P ALT_EXPAND(Act, E, 40), ALT_REP_OF_39(Act, E,  , S)
#define ALT_REP_OF_39(Act, E, P, S)     P ALT_EXPAND(Act, E, 39), ALT_REP_OF_38(Act, E,  , S)
#define ALT_REP_OF_38(Act, E, P, S)     P ALT_EXPAND(Act, E, 38), ALT_REP_OF_37(Act, E,  , S)
#define ALT_REP_OF_37(Act, E, P, S)     P ALT_EXPAND(Act, E, 37), ALT_REP_OF_36(Act, E,  , S)
#define ALT_REP_OF_36(Act, E, P, S)     P ALT_EXPAND(Act, E, 36), ALT_REP_OF_35(Act, E,  , S)
#define ALT_REP_OF_35(Act, E, P, S)     P ALT_EXPAND(Act, E, 35), ALT_REP_OF_34(Act, E,  , S)
#define ALT_REP_OF_34(Act, E, P, S)     P ALT_EXPAND(Act, E, 34), ALT_REP_OF_33(Act, E,  , S)
#define ALT_REP_OF_33(Act, E, P, S)     P ALT_EXPAND(Act, E, 33), ALT_REP_OF_32(Act, E,  , S)
#define ALT_REP_OF_32(Act, E, P, S)     P ALT_EXPAND(Act, E, 32), ALT_REP_OF_31(Act, E,  , S)
#define ALT_REP_OF_31(Act, E, P, S)     P ALT_EXPAND(Act, E, 31), ALT_REP_OF_30(Act, E,  , S)
#define ALT_REP_OF_30(Act, E, P, S)     P ALT_EXPAND(Act, E, 30), ALT_REP_OF_29(Act, E,  , S)
#define ALT_REP_OF_29(Act, E, P, S)     P ALT_EXPAND(Act, E, 29), ALT_REP_OF_28(Act, E,  , S)
#define ALT_REP_OF_28(Act, E, P, S)     P ALT_EXPAND(Act, E, 28), ALT_REP_OF_27(Act, E,  , S)
#define ALT_REP_OF_27(Act, E, P, S)     P ALT_EXPAND(Act, E, 27), ALT_REP_OF_26(Act, E,  , S)
#define ALT_REP_OF_26(Act, E, P, S)     P ALT_EXPAND(Act, E, 26), ALT_REP_OF_25(Act, E,  , S)
#define ALT_REP_OF_25(Act, E, P, S)     P ALT_EXPAND(Act, E, 25), ALT_REP_OF_24(Act, E,  , S)
#define ALT_REP_OF_24(Act, E, P, S)     P ALT_EXPAND(Act, E, 24), ALT_REP_OF_23(Act, E,  , S)
#define ALT_REP_OF_23(Act, E, P, S)     P ALT_EXPAND(Act, E, 23), ALT_REP_OF_22(Act, E,  , S)
#define ALT_REP_OF_22(Act, E, P, S)     P ALT_EXPAND(Act, E, 22), ALT_REP_OF_21(Act, E,  , S)
#define ALT_REP_OF_21(Act, E, P, S)     P ALT_EXPAND(Act, E, 21), ALT_REP_OF_20(Act, E,  , S)
#define ALT_REP_OF_20(Act, E, P, S)     P ALT_EXPAND(Act, E, 20), ALT_REP_OF_19(Act, E,  , S)
#define ALT_REP_OF_19(Act, E, P, S)     P ALT_EXPAND(Act, E, 19), ALT_REP_OF_18(Act, E,  , S)
#define ALT_REP_OF_18(Act, E, P, S)     P ALT_EXPAND(Act, E, 18), ALT_REP_OF_17(Act, E,  , S)
#define ALT_REP_OF_17(Act, E, P, S)     P ALT_EXPAND(Act, E, 17), ALT_REP_OF_16(Act, E,  , S)
#define ALT_REP_OF_16(Act, E, P, S)     P ALT_EXPAND(Act, E, 16), ALT_REP_OF_15(Act, E,  , S)
#define ALT_REP_OF_15(Act, E, P, S)     P ALT_EXPAND(Act, E, 15), ALT_REP_OF_14(Act, E,  , S)
#define ALT_REP_OF_14(Act, E, P, S)     P ALT_EXPAND(Act, E, 14), ALT_REP_OF_13(Act, E,  , S)
#define ALT_REP_OF_13(Act, E, P, S)     P ALT_EXPAND(Act, E, 13), ALT_REP_OF_12(Act, E,  , S)
#define ALT_REP_OF_12(Act, E, P, S)     P ALT_EXPAND(Act, E, 12), ALT_REP_OF_11(Act, E,  , S)
#define ALT_REP_OF_11(Act, E, P, S)     P ALT_EXPAND(Act, E, 11), ALT_REP_OF_10(Act, E,  , S)
#define ALT_REP_OF_10(Act, E, P, S)     P ALT_EXPAND(Act, E, 10), ALT_REP_OF_9(Act, E,  , S)
#define ALT_REP_OF_9(Act, E, P, S)     P ALT_EXPAND(Act, E, 9), ALT_REP_OF_8(Act, E,  , S)
#define ALT_REP_OF_8(Act, E, P, S)     P ALT_EXPAND(Act, E, 8), ALT_REP_OF_7(Act, E,  , S)
#define ALT_REP_OF_7(Act, E, P, S)     P ALT_EXPAND(Act, E, 7), ALT_REP_OF_6(Act, E,  , S)
#define ALT_REP_OF_6(Act, E, P, S)     P ALT_EXPAND(Act, E, 6), ALT_REP_OF_5(Act, E,  , S)
#define ALT_REP_OF_5(Act, E, P, S)     P ALT_EXPAND(Act, E, 5), ALT_REP_OF_4(Act, E,  , S)
#define ALT_REP_OF_4(Act, E, P, S)     P ALT_EXPAND(Act, E, 4), ALT_REP_OF_3(Act, E,  , S)
#define ALT_REP_OF_3(Act, E, P, S)     P ALT_EXPAND(Act, E, 3), ALT_REP_OF_2(Act, E,  , S)
#define ALT_REP_OF_2(Act, E, P, S)     P ALT_EXPAND(Act, E, 2), ALT_REP_OF_1(Act, E,  , S)
#define ALT_REP_OF_1(Act, E, P, S)     P ALT_EXPAND(Act, E, 1) S
#define ALT_REP_OF_00(Act, E, P, S)     LAST_ ## Act(E, 00)

/*
 * The actions we apply with REP_*
 *
 * THOR_TYPEACTION:      Declares a type to hold the name and a pointer to the internal object.
 * THOR_VALUEACTION:     Declares an initialization of the Type putting the name and the pointer
 *                  into the object
 */
#define BUILDTEMPLATETYPEPARAM(Act, Count)      ALT_REP_OF_N(Act, ,  ,  , Count)
#define BUILDTEMPLATETYPEVALUE(Act, Count)      ALT_REP_OF_N(Act, , <, >, Count)


#define THOR_TYPEACTION(TC, Type, Member)       std::pair<char const*, decltype(&Type BUILDTEMPLATETYPEVALUE(THOR_TYPENAMEVALUEACTION, TC) ::Member)>
#define THOR_VALUEACTION(TC, Type, Member)      { QUOTE(Member), &Type BUILDTEMPLATETYPEVALUE(THOR_TYPENAMEVALUEACTION, TC) ::Member }
#define THOR_NAMEACTION(TC, Type, Member)       { Type::Member, #Member ## s}
#define LAST_THOR_TYPEACTION(TC, Type)
#define LAST_THOR_VALUEACTION(TC, Type)
#define LAST_THOR_NAMEACTION(TC, Type)

#define THOR_TYPENAMEPARAMACTION(Ex, Id)        typename T ## Id
#define THOR_TYPENAMEVALUEACTION(Ex, Id)        T ## Id
#define THOR_CHECK_ASSERT(Ex, Id)
#define LAST_THOR_TYPENAMEPARAMACTION(Ex, Id)
#define LAST_THOR_TYPENAMEVALUEACTION(Ex, Id)
#define LAST_THOR_CHECK_ASSERT(Ex, Id)          DO_ASSERT(Ex)


#define THOR_MERGE_LABEL_NAME(Pre, Post)        Pre ## Post
#define THOR_UNIQUE_LABEL(Line)                 THOR_MERGE_LABEL_NAME(thorUniqueName, Line)
#define THOR_UNIQUE_NAME                        THOR_UNIQUE_LABEL(__COUNTER__)

/*
 * Defines a trait for a user defined type.
 * Lists the members of the type that can be serialized.
 */
#define DO_ASSERT(DataType)                                             \
static_assert(                                                          \
    ::ThorsAnvil::Serialize::Traits<DataType>::type != ThorsAnvil::Serialize::TraitType::Invalid,   \
    "The macro ThorsAnvil_MakeTrait must be used outside all namespace."\
)

#define ThorsAnvil_PointerAllocator(DataType, ActionObj)                \
namespace ThorsAnvil { namespace Serialize {                            \
template<>                                                              \
class Traits<DataType*>                                                 \
{                                                                       \
    public:                                                             \
        static constexpr TraitType type = TraitType::Pointer;           \
        static DataType* alloc()    {return ActionObj.alloc();}         \
        static void release(T* p)   {ActionObj.release(p);}             \
};                                                                      \
}}
#define ThorsAnvil_MakeTrait_Base(ParentType, TType, Count, DataType, ...)  \
namespace ThorsAnvil { namespace Serialize {                            \
template<BUILDTEMPLATETYPEPARAM(THOR_TYPENAMEPARAMACTION, Count)>       \
class Traits<DataType BUILDTEMPLATETYPEVALUE(THOR_TYPENAMEVALUEACTION, Count) > \
{                                                                       \
    public:                                                             \
        static constexpr TraitType type = TraitType::TType;             \
        ParentType                                                      \
        using MyType = DataType BUILDTEMPLATETYPEVALUE(THOR_TYPENAMEVALUEACTION, Count); \
                                                                        \
        using Members = std::tuple<                                     \
                        REP_N(THOR_TYPEACTION, Count, DataType, __VA_ARGS__)        \
                                    >;                                  \
                                                                        \
        static Members const& getMembers()                              \
        {                                                               \
            static constexpr Members members{                           \
                        REP_N(THOR_VALUEACTION, Count, DataType, __VA_ARGS__)       \
                                            };                          \
            return members;                                             \
        }                                                               \
                                                                        \
        template<typename M>                                            \
        static std::size_t addSizeOneMember(PrinterInterface& printer, MyType const& /*object*/, M* staticObjPtr) \
        {                                                               \
            using MemberType    = std::decay_t<M>;                      \
            if (staticObjPtr)                                           \
            {                                                           \
                return Traits<MemberType>::getPrintSize(printer, *staticObjPtr, false);\
            }                                                           \
            return printer.getSizeNull();                               \
        }                                                               \
        template<typename M>                                            \
        static std::size_t addSizeOneMember(PrinterInterface& printer, MyType const& object, M MyType::* memPtr) \
        {                                                               \
            using MemberTypeDec = decltype(object.*memPtr);             \
            using MemberType    = std::decay_t<MemberTypeDec>;          \
            return Traits<MemberType>::getPrintSize(printer, object.*memPtr, false);\
        }                                                               \
                                                                        \
        template<std::size_t... Seq>                                    \
        static std::size_t addSizeEachMember(PrinterInterface& printer, MyType const& object, std::index_sequence<Seq...> const&) \
        {                                                               \
            Members const& members = getMembers();                      \
            std::size_t result = 0;                                     \
            auto partSize = {std::size_t(0), addSizeOneMember(printer, object, std::get<Seq>(members).second)...};\
            auto nameSize = {std::size_t(0), strlen(std::get<Seq>(members).first)...};\
            for (auto val: partSize)                                     \
            {                                                           \
                result += val;                                          \
            }                                                           \
            for (auto val: nameSize)                                     \
            {                                                           \
                result += val;                                          \
            }                                                           \
            return result;                                              \
        }                                                               \
                                                                        \
        static std::size_t getPrintSizeTotal(PrinterInterface& printer, MyType const& object, std::size_t& count, std::size_t& memberSize)\
        {                                                               \
            count       += std::tuple_size_v<Members>;                  \
            memberSize  += addSizeEachMember(printer, object, std::make_index_sequence<std::tuple_size_v<Members>>());\
                                                                        \
            CalcSizeHelper<MyType>  calcHelper;                         \
            return calcHelper.getPrintSize(printer, object, count, memberSize);\
        }                                                               \
                                                                        \
        static std::size_t getPrintSize(PrinterInterface& printer, MyType const& object, bool poly)\
        {                                                               \
            return tryGetPolyMorphicPrintSize(printer, object, poly, 0);\
        }                                                               \
};                                                                      \
}}                                                                      \
ALT_REP_OF_N(THOR_CHECK_ASSERT, DataType, , , Count)

#define ThorsAnvil_RegisterPolyMorphicType_Internal(DataType, ...)      \
    ThorsAnvil_RegisterPolyMorphicType(DataType)

#pragma vera-pushoff
#define ThorsAnvil_RegisterPolyMorphicType(DataType)                    \
namespace ThorsAnvil { namespace Serialize {                            \
namespace                                                               \
{                                                                       \
    ThorsAnvil_InitPolyMorphicType<DataType>   THOR_UNIQUE_NAME ( # DataType); \
}                                                                       \
}}
#pragma vera-pop

#define ThorsAnvil_Parent(Count, ParentType, DataType, ...) using Parent = ParentType; \
                                                            using Root   = typename GetRootType<ParentType>::Root;

#define ThorsAnvil_Template_MakeTrait(Count, ...)                       \
    ThorsAnvil_MakeTrait_Base( , Map, Count, __VA_ARGS__, 1)            \
    static_assert(true, "")

#define ThorsAnvil_MakeTrait(...)                                       \
    ThorsAnvil_MakeTrait_Base( , Map, 00, __VA_ARGS__, 1);               \
    ThorsAnvil_RegisterPolyMorphicType_Internal(__VA_ARGS__, 1)         \
    static_assert(true, "")

#define ThorsAnvil_MakeTraitCustom(DataType)                            \
namespace ThorsAnvil { namespace Serialize {                            \
template<>                                                              \
class Traits<DataType>                                                  \
{                                                                       \
    public:                                                             \
    static constexpr TraitType type = TraitType::Serialize;             \
    static std::size_t getPrintSize(PrinterInterface& printer, DataType const& value, bool)\
    {                                                                   \
        using DataTypeSerializer = typename DataType::ThorsSerializer;  \
        std::size_t size = DataTypeSerializer::size(value);             \
        return printer.getSizeRaw(size);                                \
    }                                                                   \
};                                                                      \
}}                                                                      \
DO_ASSERT(DataType)

#define ThorsAnvil_Template_ExpandTrait(Count, ParentType, ...)         \
    ThorsAnvil_MakeTrait_Base(ThorsAnvil_Parent(Count, ParentType, __VA_ARGS__, 1), Parent, Count, __VA_ARGS__, 1) \
    static_assert(true, "")

#define ThorsAnvil_ExpandTrait(ParentType, ...)                     ThorsAnvil_ExpandTrait_Base(ParentType, __VA_ARGS__, 1)
#define ThorsAnvil_ExpandTrait_Base(ParentType, DataType, ...)          \
    static_assert(                                                      \
        std::is_base_of<typename ThorsAnvil::Serialize::GetPrimaryParentType<ParentType>::type, DataType>::value,                  \
        "ParentType must be a base class of DataType");                 \
    static_assert(                                                      \
        ::ThorsAnvil::Serialize::Traits<ParentType>::type != ThorsAnvil::Serialize::TraitType::Invalid, \
        "Parent type must have Serialization Traits defined"            \
    );                                                                  \
    ThorsAnvil_MakeTrait_Base(ThorsAnvil_Parent(0, ParentType, DataType, __VA_ARGS__), Parent, 00, DataType, __VA_ARGS__); \
    ThorsAnvil_RegisterPolyMorphicType_Internal(DataType, 1)            \
    static_assert(true, "")

#include <map>
#include <string>
#define ThorsAnvil_MakeEnum(EnumName, ...)                              \
namespace ThorsAnvil { namespace Serialize {                            \
template<>                                                              \
class Traits<EnumName>                                                  \
{                                                                       \
    public:                                                             \
        static constexpr    TraitType       type = TraitType::Enum;     \
        static std::map<EnumName, std::string> const& getValues()       \
        {                                                               \
            using std::string_literals::operator""s;                    \
            static const std::map<EnumName, std::string> values =       \
            {                                                           \
                REP_N(THOR_NAMEACTION, 0, EnumName, __VA_ARGS__, 1)     \
            };                                                          \
            return values;                                              \
        }                                                               \
        static std::size_t getSize()                                    \
        {                                                               \
            return NUM_ARGS(__VA_ARGS__, 1);                            \
        }                                                               \
        static EnumName getValue(std::string const& val, std::string const& msg) \
        {                                                               \
            std::map<EnumName, std::string> const& values = getValues();\
            for (auto const& value: values)                             \
            {                                                           \
                if (val == value.second) {                              \
                    return value.first;                                 \
                }                                                       \
            }                                                           \
            throw std::runtime_error(msg + " Invalid Enum Value");      \
        }                                                               \
        static std::size_t getPrintSize(PrinterInterface& printer, EnumName const& value, bool)\
        {                                                               \
            auto find = getValues().find(value);                        \
            return printer.getSizeValue(find->second);                  \
        }                                                               \
};                                                                      \
}}                                                                      \
DO_ASSERT(EnumName)

/*
 * Defined the virtual function needed by tryPrintPolyMorphicObject()
 */
#define ThorsAnvil_PolyMorphicSerializer(Type)                                              \
    virtual void printPolyMorphicObject(ThorsAnvil::Serialize::Serializer&         parent,  \
                                       ThorsAnvil::Serialize::PrinterInterface&    printer) \
    {                                                                                       \
        ThorsAnvil::Serialize::printPolyMorphicObject<Type>(parent, printer, *this);        \
    }                                                                                       \
    virtual void parsePolyMorphicObject(ThorsAnvil::Serialize::DeSerializer&       parent,  \
                                       ThorsAnvil::Serialize::ParserInterface&     parser)  \
    {                                                                                       \
        ThorsAnvil::Serialize::parsePolyMorphicObject<Type>(parent, parser, *this);         \
    }                                                                                       \
    virtual std::size_t getPolyMorphicPrintSize(ThorsAnvil::Serialize::PrinterInterface& printer) const \
    {                                                                                       \
        std::size_t count = 1;                                                              \
        std::size_t memberSize = (printer.config.polymorphicMarker.size() + printer.getSizeValue(std::string(polyMorphicSerializerName())));\
                                                                                            \
        return getNormalPrintSize(printer, *this, count, memberSize);                       \
    }                                                                                       \
    static constexpr char const* polyMorphicSerializerName()                                \
    {                                                                                       \
        return #Type;                                                                       \
    }


namespace ThorsAnvil
{
    namespace Serialize
    {

/*
 * The traits type.
 * Specialized for each type we want to serialize
 */
template<typename T>
class Traits
{
    public:
        // By default if a traits type is not defined for a type
        // You get this default implementation.
        // This just says that it is invalid to serialize this type
        // static_asserts sprinkled through the code will provide
        // appropriate error messages based on this being invalid.
        static constexpr TraitType type = TraitType::Invalid;

        // For types that are Map/Array/Parent we can also define
        // members that need be serialized via types. To achieve this
        // they  define a static getMembers() function.
        // static Members const& getMembers()
        //
        // I would have made this a type declaration but I could
        // not get it to bind correctly without an explicit
        // instantiation (if you know how please let me know).
        //
        // So I use a static member function with a static variable
        // which can be defined in-line within the traits class and
        // does not need a separate declaration in a compilation unit.
};


/*
 * For object that are serialized as Json Array
 * we use this object to get the size of the array.
 *
 * The default action is simply to call size() on
 * the object before printing.
 */
template<typename T>
class SerializeArraySize
{
    public:
        static std::size_t size(T const& object)
        {
            return object.size();
        }
};

template<typename T, typename R = T>
struct GetRootType
{
    using Root = R;
};
template<typename T>
struct GetRootType<T, typename Traits<T>::Root>
{
    using Root = typename Traits<T>::Root;
};
template<typename T>
struct GetAllocationType
{
    using AllocType = T;
};
template<typename T>
struct GetAllocationType<std::unique_ptr<T>>
{
    using AllocType = T;
};

/*
 */
class PolyMorphicRegistry
{
    static std::map<std::string, std::function<void*()>>& getContainer()
    {
        static std::map<std::string, std::function<void*()>>    polyAllocContainer;
        return polyAllocContainer;
    }

    public:
        static std::function<void*()>& getAllocator(std::string const& name)
        {
            return getContainer()[name];
        }
        template<typename T>
        static T* getNamedTypeConvertedTo(std::string const& name)
        {
            using AllocType     = typename GetAllocationType<T>::AllocType;

            auto     cont       = getContainer();
            auto     find       = cont.find(name);
            if (find == cont.end())
            {
                throw std::runtime_error("ThorsAnvil::Serialize::PolyMorphicRegistry::getNamedTypeConvertedTo: Non polymorphic type");
            }
            void*       data        = find->second();
            AllocType*  dataBase    = reinterpret_cast<AllocType*>(data);

            using ReturnType    = T*;
            return ReturnType{dataBase};
        }
};

template <typename T>
class HasPolyMorphicObjectMarker
{
    using One = char;
    using Two = long;

    template <typename C> static One test( decltype(&C::parsePolyMorphicObject) );
    template <typename C> static Two test(...);

public:
    enum { Value = sizeof(test<T>(nullptr)) == sizeof(char) };
};


template<typename T, bool Poly = HasPolyMorphicObjectMarker<T>::Value>
struct ThorsAnvil_InitPolyMorphicType;


template<typename T>
struct ThorsAnvil_InitPolyMorphicType<T, true>
{
    ThorsAnvil_InitPolyMorphicType(char const* name)
    {
        PolyMorphicRegistry::getAllocator(name) =
            []() -> void*
            {
                using Root = typename GetRootType<T>::Root;
                return dynamic_cast<Root*>(Traits<T*>::alloc());
            };
    }
};

template<typename T>
struct ThorsAnvil_InitPolyMorphicType<T, false>
{
    ThorsAnvil_InitPolyMorphicType(char const*){}
};


    }
}

#endif
