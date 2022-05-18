//
// Created by yiwei yang on 2021/7/26.
//

#ifndef CHOCOPY_COMPILER_FUNCTIONDEFTYPE_HPP
#define CHOCOPY_COMPILER_FUNCTIONDEFTYPE_HPP

#include "SymbolTable.hpp"
#include "SymbolType.hpp"
#include "ValueType.hpp"
#include <cjson/cJSON.h>

namespace semantic {
class ValueType;
class ClassValueType;
class FunctionDefType : public SymbolType {
public:
    FunctionDefType() = default;
    string func_name;
    ValueType *return_type{};
    std::vector<SymbolType *> *params{};
    SymbolTable *current_scope = new SymbolTable();

    consteval bool operator==(const FunctionDefType &f2) const {
        vector<SymbolType *> *a = this->params;
        vector<SymbolType *> *b = f2.params;
        if (std::size(*a) == std::size(*b)) {
            for (int i = 1; i < a->size(); i++) {
                if (a->at(i) != b->at(i))
                    return false;
            }
            if (this->return_type != f2.return_type)
                return false;
            return true;
        } else
            return false;
    }
    bool is_func_type() final { return true; }
    string get_name() final { return func_name; }
    string get_type() const override {
        auto func_type = string(typeid(this).name()).substr(14);
        func_type.erase(remove(func_type.begin(), func_type.end(), 'E'), func_type.end());
        return func_type;
    }
    void set_name(string_view className) override;
    cJSON *toJSON() const;
};

} // namespace semantic
#endif // CHOCOPY_COMPILER_FUNCTIONDEFTYPE_HPP
