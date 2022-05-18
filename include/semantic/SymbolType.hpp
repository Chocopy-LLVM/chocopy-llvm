//
// Created by yiwei yang on 2/18/21.
//

#ifndef CHOCOPY_COMPILER_SYMBOLTYPE_HPP
#define CHOCOPY_COMPILER_SYMBOLTYPE_HPP

#include <string>
#include <string_view>

using std::string;
using std::string_view;

namespace semantic {
class SymbolType {
public:
    virtual constexpr bool is_value_type() { return false; }
    virtual constexpr bool is_list_type() { return false; }
    virtual constexpr bool is_func_type() { return false; }
    virtual constexpr bool is_none_type() { return false; }
    virtual constexpr bool is_special_type() { return false; }

    virtual string get_name() = 0;
    virtual void set_name(string_view className) = 0;
    virtual string get_type() const = 0;

    template <typename _Ty> bool eq(const _Ty &_Value);
    template <typename _Ty> bool neq(const _Ty &_Value);
};
} // namespace semantic

#endif // CHOCOPY_COMPILER_SYMBOLTYPE_HPP
