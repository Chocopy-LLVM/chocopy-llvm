//
// Created by yiwei yang on 2/18/21.
//

#include "FunctionDefType.hpp"
#include "ValueType.hpp"
#include <chocopy_semant.hpp>
#include <fmt/format.h>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <utility>

using std::map;
using std::set;

bool is_lambda_pass = false;
bool is_rvalue = false;
vector<string> lambda_params;
vector<string> conslist_table;
vector<string> len_list_table;
template <class Key, class T> set<string> key_to_set(const std::map<Key, T> &map, std::set<Key> &set) {
    set.clear();
    auto itr = map.begin();
    while (map.end() != itr) {
        set.insert((itr++)->first);
    }
    return set;
}

namespace semantic {
cJSON *FunctionDefType::toJSON() const {
    cJSON *d = cJSON_CreateObject();

    cJSON_AddStringToObject(d, "kind", "FuncType");
    auto param = cJSON_CreateArray();

    if (!this->params->empty())
        for (auto &parameter : *this->params) {
            auto type_name = parameter->get_type().c_str();
            auto tmp_param = cJSON_CreateObject();

            if (dynamic_cast<ListValueType *>(parameter)) {
                cJSON_AddStringToObject(tmp_param, "kind", type_name);
                cJSON_AddItemToObject(tmp_param, "elementType",
                                      parser::add_inferred_type(((ListValueType *)parameter)->element_type));
                cJSON_AddItemToArray(param, tmp_param);
            } else {
                auto param_name = ((ClassValueType *)parameter)->class_name.c_str();
                cJSON_AddStringToObject(tmp_param, "kind", type_name);
                cJSON_AddStringToObject(tmp_param, "className", param_name);
                cJSON_AddItemToArray(param, tmp_param);
            }
        }
    cJSON_AddItemToObject(d, "parameters", param);

    auto class_name = ((ClassValueType *)this->return_type)->class_name.c_str();
    auto tmp_return = cJSON_CreateObject();
    cJSON_AddStringToObject(tmp_return, "kind", "ClassValueType");
    cJSON_AddStringToObject(tmp_return, "className", class_name);
    cJSON_AddItemToObject(d, "returnType", tmp_return);
    return d;
}
void FunctionDefType::set_name(string_view className) { ((ClassValueType *)this->return_type)->class_name = className; }
template <typename Ty> bool SymbolType::eq(const Ty &Value) {
    if (this->is_list_type()) {
        if (dynamic_cast<ListValueType *>(Value))
            return ((ListValueType *)this)->element_type->eq(((ListValueType *)Value)->element_type);
        else
            return false;
    } else if (this->is_value_type()) {
        if (dynamic_cast<ClassValueType *>(Value))
            return ((ClassValueType *)this)->get_name() == ((ClassValueType *)Value)->get_name();
        else
            return false;
    }
    return false;
}
template <typename Ty> bool SymbolType::neq(const Ty &Value) { return !(this->eq(Value)); }

void DeclarationAnalyzer::debug_sym() {
    for (const auto &x : *sym->tab) {
        if ((x.second)->is_list_type()) {
            LOG(INFO) << x.first << " : [" << ((ClassValueType *)((ListValueType *)x.second)->element_type)->get_name()
                      << "]\n";
        } else if ((x.second)->is_func_type()) {
            if (dynamic_cast<FunctionDefType *>(x.second))
                LOG(INFO) << "Function " << x.first << " : "
                          << dynamic_cast<FunctionDefType *>(x.second)->return_type->get_name() << "\nparam\n";
            for (const auto y : *dynamic_cast<FunctionDefType *>(x.second)->params)
                if (!y->is_list_type())
                    LOG(INFO) << "      : " << ((ClassValueType *)y)->get_name() << "\n";
                else
                    LOG(INFO) << "      : " << ((ClassValueType *)((ListValueType *)y)->element_type)->get_name()
                              << "\n";
        } else if ((x.second)->is_value_type()) {
            LOG(INFO) << x.first << " : " << ((ClassValueType *)x.second)->get_name() << "\n";
        }
    }
}
void DeclarationAnalyzer::visit(parser::VarDef &varDef) {
    if (dynamic_cast<parser::ClassType *>(varDef.var->type) &&
        sym->get<SymbolType *>(((parser::ClassType *)varDef.var->type)->className) == nullptr &&
        !(((parser::ClassType *)varDef.var->type)->className == curr_class)) {
        errors->emplace_back(
            new SemanticError(varDef.var->type, fmt::format("Invalid type annotation; there is no class named: {}",
                                                            ((parser::ClassType *)varDef.var->type)->get_name())));
    }
    this->passing_type = ValueType::annotate_to_val(varDef.var->type);
}
/* The Declaration check rules Your Code Here */

template <typename... Args> void TypeChecker::typeError(parser::Node *node, const string &message, Args... rest) {
    node->semError(rest...);
    node->semError(message);
}
void TypeChecker::typeError(parser::Node *node, const string &message) {
    if (!node->has_type_err()) {
        node->typeError = message;
    } else {
        node->typeError += "\t" + message;
    }
}
void TypeChecker::visit(parser::Program &program) {
    for (auto decl : *program.declarations) {
        decl->accept(*this);
    }
    for (auto stmt : *program.statements) {
        stmt->accept(*this);
    }
}
/* The Type check rules Your Code Here */

void TypeChecker::debug_sym() {
    for (const auto &x : *sym->tab) {
        if ((x.second)->is_list_type()) {
            LOG(INFO) << x.first << " : [" << ((ClassValueType *)((ListValueType *)x.second)->element_type)->get_name()
                      << "]\n";
        } else if ((x.second)->is_func_type()) {
            if (dynamic_cast<FunctionDefType *>(x.second)) {
                LOG(INFO) << "Function " << x.first << " : [ "
                          << dynamic_cast<FunctionDefType *>(x.second)->return_type->get_name() << " ]";
                debug_nested_func_sym(((FunctionDefType *)x.second)->current_scope, 1);
            }
            for (const auto y : *dynamic_cast<FunctionDefType *>(x.second)->params)
                if (!y->is_list_type())
                    LOG(INFO) << "      : " << ((ClassValueType *)y)->get_name() << "\n";
                else
                    LOG(INFO) << "      : " << ((ClassValueType *)((ListValueType *)y)->element_type)->get_name()
                              << "\n";
        } else if ((x.second)->is_value_type()) {
            LOG(INFO) << x.first << " : " << ((ClassValueType *)x.second)->get_name() << "\n";
        }
    }
}

void TypeChecker::debug_nested_func_sym(SymbolTable *func_sym, int tab) {
    for (auto x : *func_sym->tab) {
        if ((x.second)->is_list_type()) {
            LOG(INFO) << fmt::format("{:>{}} : [ {} ]", x.first, tab * 20,
                                     ((ClassValueType *)((ListValueType *)x.second)->element_type)->get_name());
        } else if ((x.second)->is_func_type()) {
            if (dynamic_cast<FunctionDefType *>(x.second)) {
                LOG(INFO) << fmt::format("{:>{}} : [ {} ]", "Function " + x.first, tab * 20,
                                         dynamic_cast<FunctionDefType *>(x.second)->return_type->get_name());
                debug_nested_func_sym(((FunctionDefType *)x.second)->current_scope, tab++);
            }
            for (const auto y : *dynamic_cast<FunctionDefType *>(x.second)->params)
                if (!y->is_list_type())
                    LOG(INFO) << fmt::format("{:>{}}       : [ {} ]", "", tab * 20, ((ClassValueType *)y)->get_name());
                else
                    LOG(INFO) << fmt::format("{:>{}}       : [ {} ]", "", tab * 20,
                                             ((ClassValueType *)((ListValueType *)y)->element_type)->get_name());
        } else if ((x.second)->is_value_type()) {
            LOG(INFO) << fmt::format("{:>{}} : [ {} ]", x.first, tab * 20, ((ClassValueType *)x.second)->get_name());
        }
    }
}
void TypeChecker::visit(parser::IfExpr &s) {
    s.condition->accept(*this);
    auto *condType = dynamic_cast<ValueType *>(this->passing_type);
    s.thenExpr->accept(*this);
    s.elseExpr->accept(*this);
}
/** Get the right type */
string TypeChecker::get_common_type(SymbolType *first, SymbolType *second) {
    string tmp = first->get_name();
    if (tmp.starts_with("[")) {
        if (second->get_name() == "object")
            return "object";
        else
            return "";
    }
    list<string> first_dfs = {tmp};
    while (tmp != "object") {
        for (const auto &x : super_classes) {
            if (tmp == x.first) {
                first_dfs.push_back(x.second);
                tmp = x.second;
                break;
            }
        }
    }
    tmp = second->get_name();
    while (tmp != "object") {
        if (std::find(first_dfs.begin(), first_dfs.end(), tmp) != first_dfs.end()) {
            return tmp;
        }
        for (const auto &x : super_classes) {
            if (tmp == x.first) {
                tmp = x.second;
                break;
            }
            if (tmp == "None") {
                return "object";
            }
        }
    }
    return "object";
}
void TypeChecker::setup_num_to_class() {
    for (const auto &x : *sym->tab) {
        if (dynamic_cast<ClassDefType *>(x.second)) {
            auto tmp_dad = sym->get<ClassDefType *>(((ClassDefType *)x.second)->get_dad());
            super_classes[x.second->get_name()] = ((ClassDefType *)x.second)->get_dad();
            add_edge(((ClassDefType *)x.second)->get_dad(), x.second->get_name());
            while (tmp_dad != nullptr && tmp_dad->get_name() != "object" &&
                   !super_classes.contains(tmp_dad->get_name())) {
                add_edge(tmp_dad->get_dad(), tmp_dad->get_name());
                tmp_dad = sym->get<ClassDefType *>(tmp_dad->get_dad());
            }
        }
    }
    dfs("object");
    sym->class_tag_["list"] = -1;
    sym->class_tag_["object"] = 0;
    sym->class_tag_["int"] = 1;
    sym->class_tag_["bool"] = 2;
    sym->class_tag_["str"] = 3;
}
/** Only takes care of non list */
bool TypeChecker::is_sub_type(SymbolType *first, SymbolType *second) {
    return get_common_type(first, second) == first->get_name();
}
ValueType *ValueType::annotate_to_val(parser::TypeAnnotation *annotation) {
    if (dynamic_cast<parser::ClassType *>(annotation)) {
        return new ClassValueType((parser::ClassType *)annotation);
    } else {
        if (annotation != nullptr && annotation->kind == "<None>")
            return new ClassValueType("<None>");
        if (dynamic_cast<parser::ListType *>(annotation))
            return new ListValueType((parser::ListType *)annotation);
    }
    return nullptr;
}

ListValueType::ListValueType(parser::ListType *typeAnnotation)
    : element_type(ValueType::annotate_to_val(typeAnnotation->elementType)) {}

ClassValueType::ClassValueType(parser::ClassType *classTypeAnnotation) : class_name(classTypeAnnotation->className) {}

cJSON *SemanticError::toJSON() {
    cJSON *d = parser::Err::toJSON();
    cJSON_AddStringToObject(d, "message", this->message.c_str());
    return d;
}
string ValueType::get_name() { return ((ClassValueType *)this)->class_name; }
} // namespace semantic

#ifdef PA2
int main(int argc, char *argv[]) {
    parser::Program *tree = parse(argv[1]);

    auto error = new vector<parser::Err *>();

    auto *declarationAnalyzer = new semantic::DeclarationAnalyzer(error);

    tree->accept(*declarationAnalyzer);
    semantic::SymbolTable *globalScope = declarationAnalyzer->getGlobals();
    error = declarationAnalyzer->errors;
    if (!error->empty()) {
        tree->add_error(error);
    } else {
        auto *typeChecker = new semantic::TypeChecker(globalScope, error);
        tree->accept(*typeChecker);
    }

    cJSON *a = tree->toJSON();
    char *out = cJSON_Print(a);
    cout << out;
}
#endif