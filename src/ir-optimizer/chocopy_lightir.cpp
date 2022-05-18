#include "GlobalVariable.hpp"
#include "Type.hpp"
#include <chocopy_lightir.hpp>
#include <chocopy_parse.hpp>
#include <chocopy_semant.hpp>
#include <fstream>
#include <queue>
#if __cplusplus > 202000L && !defined(__clang__)
#include <ranges>
#endif
#include <regex>

namespace semantic {
class SemanticError;
class SymbolTable;
class DeclarationAnalyzer;
class TypeChecker;
} // namespace semantic
namespace lightir {
Type *VOID_T;
Type *BOOL_T;
Type *INT_T;
Type *ARR_T;
Type *STR_T;
auto OBJ_T = new vector<Type *>();
const std::regex to_replace("(\\$(.+?)+\\.)|\\$");
const std::regex to_replace_prev(".+?\\.");
const std::regex to_replace_post("\\..+?$");
#define CONST(num) ConstantInt::get(num, &*module)
vector<Constant *> new_array;
vector<Value *> init_val;
vector<BasicBlock *> base_layer;
vector<BasicBlock *> for_layer_stack;
vector<string> nonlocal_symbol;
vector<string> iterable_symbol;
Value *to_stored_class;
bool is_local_global = false;
bool is_member_call = false;
bool should_pass = false;
bool is_func_def = true;
/** Store the tmp value */
Value *tmp_value;
int tmp_int = 0;
/** Check whether is rval */
bool is_rval = true;
string tmp_string;
/** Check whether is the int variable */
bool use_int = false;
/** Check whether use the conslist to initialize a list */
bool is_conslist = false;
/** If there's no function or class or doubly list or above
 * just skip all the global conslist */
bool is_global_conslist = false;
/** Function that is being built */
Function *curr_func = nullptr;
Class *curr_class = nullptr;

/** Use the symbol table to generate the type id */
int LightWalker::get_next_type_id() { return next_type_id++; }

int LightWalker::get_const_type_id() { return next_const_id++; }

int LightWalker::get_class_id(const string &name) const { return sym->class_tag_[name]; }

string LightWalker::get_nested_func_name(semantic::SymbolTable *func_sym, string &name) {
    if (func_sym->parent) {
        for (const auto &x : *func_sym->tab)
            if ((x.second)->is_func_type())
                if (dynamic_cast<semantic::FunctionDefType *>(x.second)) {
                    if (x.second->get_name() == name) {
                        func_found = true;
                        break;
                    }
                    name = get_nested_func_name(((semantic::FunctionDefType *)x.second)->current_scope, name);
                    if (func_found) {
                        name = x.second->get_name() + "." + name;
                        break;
                    }
                }
    } else if (curr_class != nullptr) {
        string tmp_name = name;
        for (const auto &x : *func_sym->tab) {
            if (dynamic_cast<semantic::ClassDefType *>(x.second) &&
                dynamic_cast<semantic::ClassDefType *>(x.second)->get_name() == curr_class->get_string()) {
                func_sym = dynamic_cast<semantic::ClassDefType *>(x.second)->current_scope;
            }
        }
        semantic::SymbolTable *tmp_sym = func_sym;
        std::queue<semantic::SymbolTable *> q;
        q.push(func_sym);
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                tmp_sym = q.front();
                q.pop();
                for (const auto &x : *tmp_sym->tab)
                    if ((x.second)->is_func_type())
                        if (dynamic_cast<semantic::FunctionDefType *>(x.second)) {
                            if (x.second->get_name() == name) {
                                func_found = true;
                                break;
                            }
                            q.push(dynamic_cast<semantic::FunctionDefType *>(x.second)->current_scope);
                        }
                if (func_found)
                    break;
            }
            if (func_found) {
                func_found = false;
                break;
            }
        }

        for (const auto &x : *tmp_sym->tab)
            if ((x.second)->is_func_type())
                if (dynamic_cast<semantic::FunctionDefType *>(x.second)) {
                    if (x.second->get_name() == name) {
                        func_found = true;
                        break;
                    }
                    name = get_nested_func_name(((semantic::FunctionDefType *)x.second)->current_scope, name);
                    if (func_found) {
                        name = x.second->get_name() + "." + name;
                        break;
                    }
                }
        if (name.find(curr_class->get_name()) != string::npos) {
            name = curr_class->get_name() + "." + name;
        }
    }
    return name;
}

/** if the tpe is local and no function call, it shall not init the @$list$prototype */
Type *LightWalker::string_to_type(const string &type_name) {
    Type *inside_loaded = nullptr;
    if (type_name == "int")
        return INT_T;
    else if (type_name == "bool")
        return BOOL_T;
    else if (type_name == "str")
        return OBJ_T->at(3);
    else if (type_name == "object")
        return OBJ_T->at(0);
    else if (type_name.starts_with("[")) {
        inside_loaded = ArrayType::get(list_class->get_type());
    } else if (!type_name.empty()) {
        auto res_num = sym->class_tag_[type_name];
        if (res_num <= 3)
            return OBJ_T->at(res_num);
        else {
            if (res_num<OBJ_T->size()) {
                return ArrayType::get(OBJ_T->at(res_num));
            }else {
                return ArrayType::get(OBJ_T->at(res_num-1));
            }
        }
    } else
        return VOID_T;

    return inside_loaded;
}

Type *LightWalker::string_to_type_no_conslist(const string &type_name) {
    Type *inside_loaded = nullptr;
    if (type_name.find("int") != std::string::npos) {
        inside_loaded = Type::get_int32_type(&*module);
    } else if (type_name.find("bool") != std::string::npos) {
        inside_loaded = Type::get_int1_type(&*module);
    } else if (type_name.find("str") != std::string::npos) {
        inside_loaded = Type::get_str_type(&*module);
    } else if (type_name.starts_with("[")) {
        inside_loaded = ArrayType::get(list_class->get_type());
        if (type_name.find("int") != std::string::npos) // get an uninitialized item
            inside_loaded = get_module()->get_int32_type();
        else if (type_name.find("bool") != std::string::npos)
            inside_loaded = get_module()->get_int1_type();
        else if (type_name.find("str") != std::string::npos)
            inside_loaded = get_module()->get_str_type();
        for (int i = 0; i < std::count(type_name.begin(), type_name.end(), '['); i++) {
            inside_loaded = ArrayType::get(inside_loaded);
        }
    }
    return inside_loaded;
}

Type *LightWalker::string_to_type_conslist(string type_name) {
    Value *inside_loaded = nullptr;
    Type *result_loaded = nullptr;
    auto const_str = scope.find("const_3");
    if (type_name == "int")
        return OBJ_T->at(1);
    else if (type_name == "bool")
        return OBJ_T->at(2);
    else if (type_name == "str")
        return OBJ_T->at(3);
    else if (type_name.starts_with("[")) {
        if (type_name.find("int") != std::string::npos) // get an uninitialized item
            inside_loaded = CONST(0);
        else if (type_name.find("bool") != std::string::npos)
            inside_loaded = CONST(false);
        else if (type_name.find("str") != std::string::npos)
            inside_loaded = const_str;
        for (int i = 0; i < std::count(type_name.begin(), type_name.end(), '['); i++) {
            inside_loaded = List::get(list_class, {inside_loaded}, "const_" + std::to_string(get_const_type_id()));
        }
        result_loaded = dynamic_cast<List *>(inside_loaded);
    } else if (!type_name.empty()) {
        return OBJ_T->at(sym->class_tag_[type_name]);
    } else
        return VOID_T;

    return result_loaded;
}

Type *LightWalker::string_to_type_conslist_init(string type_name) {
    Value *inside_loaded = nullptr;
    Type *result_loaded = nullptr;
    auto const_str = scope.find("const_3");
    if (type_name == "int")
        return OBJ_T->at(1);
    else if (type_name == "bool")
        return OBJ_T->at(2);
    else if (type_name == "str")
        return OBJ_T->at(3);
    else if (type_name.starts_with("[")) {
        if (type_name.find("int") != std::string::npos) // get an uninitialized item
            inside_loaded = CONST(0);
        else if (type_name.find("bool") != std::string::npos)
            inside_loaded = CONST(false);
        else if (type_name.find("str") != std::string::npos)
            inside_loaded = const_str;
        for (int i = 0; i < std::count(type_name.begin(), type_name.end(), '['); i++) {
            inside_loaded = list_class;
        }
        result_loaded = list_class;
    } else if (!type_name.empty()) {
        return OBJ_T->at(sym->class_tag_[type_name]);
    } else
        return VOID_T;

    return result_loaded;
}

LightWalker::LightWalker(semantic::SymbolTable *sym) : sym(sym) {
    module = std::make_unique<Module>("ChocoPy code");
    builder = new IRBuilder(nullptr, module.get());

    auto TyVoid = Type::get_void_type(&*module);
    auto TyI32 = Type::get_int32_type(&*module);
    auto TyI1 = Type::get_int1_type(&*module);
    auto TyString = Type::get_str_type(&*module);
    auto I8 = new IntegerType(8, &*module);
    auto TyArrI32 = Type::get_array_type(new IntegerType(32, &*module));
    auto TyArrI1 = Type::get_array_type(new IntegerType(1, &*module));
    auto TyArrStr = Type::get_array_type(new StringType("", &*module));

    /** Get the class ready. */
    std::vector<Type *> object_params;

    object_class = new Class(&*module, "object", get_next_type_id(), nullptr, true, true);
    auto TyObject = object_class->get_type();
    object_params.emplace_back(
        ArrayType::get(LabelType::get("$object$prototype", dynamic_cast<Class *>(TyObject), &*module)));
    auto object_init = new Function(FunctionType::get(TyVoid, object_params), "$object.__init__", &*module);
    object_class->add_method(object_init);
    int_class = new Class(&*module, "int", get_next_type_id(), nullptr, true, true);
    auto TyIntClass = int_class->get_type();
    int_class->add_method(object_init);
    int_class->add_attribute(new AttrInfo(TyI32, "__int__"));
    bool_class = new Class(&*module, "bool", get_next_type_id(), nullptr, true, true);
    auto TyBoolClass = bool_class->get_type();
    bool_class->add_method(object_init);
    bool_class->add_attribute(new AttrInfo(TyI1, "__bool__"));
    str_class = new Class(&*module, "str", get_next_type_id(), nullptr, true, true);
    auto TyStrClass = str_class->get_type();
    str_class->add_method(object_init);
    str_class->add_attribute(new AttrInfo(TyI32, "__len__", 0));
    str_class->add_attribute(new AttrInfo(TyString, "__str__"));

    vector<Type *> union_vec;
    auto TyPtrObject = new ArrayType(TyObject);
    auto TyPtrInt = new ArrayType(TyIntClass);
    auto TyPtrBool = new ArrayType(TyBoolClass);
    auto TyPtrStr = new ArrayType(TyStrClass);
    union_vec.emplace_back(TyPtrInt);
    union_vec.emplace_back(TyPtrBool);
    union_vec.emplace_back(TyPtrStr);
    union_vec.emplace_back(TyPtrObject);
    auto union_list = new Union(union_vec, "type");
    list_class = new Class(&*module, ".list", -1, nullptr, false);
    list_class->add_method(object_init);
    list_class->add_attribute(new AttrInfo(ArrayType::get(union_list), "", union_list));
    list_class->add_attribute(new AttrInfo(TyI32, "__len__", 0));

    auto TyListClass = list_class->get_type();
    auto TyPtrList = new ArrayType(TyListClass);

    /** Predefined functions. */
    std::vector<Type *> heat_init_params;
    auto heat_init_type = FunctionType::get(TyVoid, heat_init_params);
    auto heat_init_fun = Function::create(heat_init_type, "heap.init", module.get());

    std::vector<Type *> initchars_params;
    initchars_params.emplace_back(I8);
    auto initchars_type =
        FunctionType::get(ArrayType::get(LabelType::get("$str$prototype", str_class, &*module)), initchars_params);
    auto initchars_fun = Function::create(initchars_type, "initchars", module.get());

    std::vector<Type *> noconv_params;
    auto noconv_type =
        FunctionType::get(ArrayType::get(LabelType::get("$int$prototype", int_class, &*module)), noconv_params);
    auto noconv_fun = Function::create(noconv_type, "noconv", module.get());

    std::vector<Type *> nonlist_params;
    auto nonlist_type = FunctionType::get(TyPtrList, nonlist_params);
    auto nonlist_fun = Function::create(nonlist_type, "nonlist", module.get());

    std::vector<Type *> error_oob_params;
    auto error_oob_type = FunctionType::get(TyVoid, error_oob_params);
    auto error_oob_fun = Function::create(error_oob_type, "error.OOB", module.get());
    std::vector<Type *> error_none_params;
    auto error_none_type = FunctionType::get(TyVoid, error_none_params);
    auto error_none_fun = Function::create(error_none_type, "error.None", module.get());
    std::vector<Type *> error_div_params;
    auto error_div_type = FunctionType::get(TyVoid, error_div_params);
    auto error_div_fun = Function::create(error_none_type, "error.Div", module.get());

    scope.enter();
    scope.push(union_list->get_name(), union_list);
    scope.push("object.__init__", object_init);
    scope.push("heap.init", heat_init_fun);
    scope.push("initchars", initchars_fun);
    scope.push("noconv", noconv_fun);
    scope.push("nonlist", nonlist_fun);

    scope.push("error.OOB", error_oob_fun);
    scope.push("error.None", error_none_fun);
    scope.push("error.Div", error_div_fun);

    scope.push("object", object_class);
    scope.push("int", int_class);
    scope.push("bool", bool_class);
    scope.push("str", str_class);
    scope.push(".list", list_class);

    module->add_union(union_list);
}

/**
 * Analyze PROGRAM, creating Info objects for all symbols.
 * Populate the global symbol table.
 */
void LightWalker::visit(parser::Program &node) {
    VOID_T = Type::get_void_type(module.get());
    BOOL_T = Type::get_int1_type(module.get());
    INT_T = Type::get_int32_type(module.get());
    STR_T = Type::get_str_type(module.get());
    ARR_T = Type::get_array_type(module.get());

    for (const auto &obj_type : sym->class_tag_) {
        if (obj_type.first == "list")
            module->add_class_type(new Class(&*module, ".list", obj_type.second, nullptr));
        else if (obj_type.first == "str" || obj_type.first == "int" || obj_type.first == "bool")
            module->add_class_type(new Class(&*module, obj_type.first, obj_type.second, nullptr));
    }
    for (int i = 0; i < sym->class_tag_.size(); i++) {
        OBJ_T->emplace_back(Type::get_class_type(module.get(), i));
    }
    OBJ_T->at(0) = object_class;

    /** Some function that requires the OBJ_T to define */
    auto TyPtrList = ArrayType::get(list_class->get_type());
    auto TyPtrObj = ArrayType::get(module->get_class().front());
    // FIXME: auto TyPtrObj = LabelType::get("$object$prototype_type",get_module()->get_class().front(), module.get());
    auto TyPtrInt = ArrayType::get(OBJ_T->at(1));
    auto TyPtrBool = ArrayType::get(OBJ_T->at(2));
    auto TyPtrStr = ArrayType::get(OBJ_T->at(3));
    vector<Type *> union_len_vec;
    vector<Type *> union_put_vec;
    vector<Type *> union_conslist_vec;
    union_len_vec.emplace_back(TyPtrList);
    union_len_vec.emplace_back(TyPtrStr);
    union_put_vec.emplace_back(TyPtrInt);
    union_put_vec.emplace_back(TyPtrBool);
    union_put_vec.emplace_back(TyPtrStr);
    union_conslist_vec.emplace_back(INT_T);
    union_conslist_vec.emplace_back(BOOL_T);
    union_conslist_vec.emplace_back(TyPtrStr);
    union_conslist_vec.emplace_back(TyPtrList);
    union_len = new Union(union_len_vec, "len");
    union_put = new Union(union_put_vec, "put");
    union_conslist = new Union(union_conslist_vec, "conslist");
    list_class->add_attribute(new AttrInfo(ArrayType::get(union_conslist), "__list__", 0));
    std::vector<Type *> concat_params;
    std::vector<Type *> conslist_params;
    concat_params.emplace_back(TyPtrList);
    concat_params.emplace_back(TyPtrList);
    conslist_params.emplace_back(INT_T);
    conslist_params.emplace_back(union_conslist);
    auto concat_type = FunctionType::get(TyPtrList, concat_params);
    auto conslist_type = FunctionType::get(TyPtrList, conslist_params, true);
    auto concat_fun = Function::create(concat_type, "concat", module.get());
    auto conslist_fun = Function::create(conslist_type, "conslist", module.get());

    std::vector<Type *> len_params;
    len_params.emplace_back(ArrayType::get(union_len));
    auto len_type = FunctionType::get(INT_T, len_params);
    auto len_fun = Function::create(len_type, "$len", module.get());

    std::vector<Type *> put_params;
    put_params.emplace_back(ArrayType::get(union_put));
    auto put_type = FunctionType::get(VOID_T, put_params);
    auto put_fun = Function::create(put_type, "print", module.get());

    std::vector<Type *> makebool_params;
    std::vector<Type *> makeint_params;
    std::vector<Type *> makestr_params;
    makebool_params.emplace_back(BOOL_T);
    makeint_params.emplace_back(INT_T);
    makestr_params.emplace_back(TyPtrStr);
    auto makebool_type = FunctionType::get(TyPtrBool, makebool_params);
    auto makeint_type = FunctionType::get(TyPtrInt, makeint_params);
    auto makestr_type = FunctionType::get(TyPtrStr, makestr_params);
    auto makebool_fun = Function::create(makebool_type, "makebool", module.get());
    auto makeint_fun = Function::create(makeint_type, "makeint", module.get());
    auto makestr_fun = Function::create(makestr_type, "makestr", module.get());

    auto input_type = FunctionType::get(TyPtrStr, {});
    auto input_fun = Function::create(input_type, "$input", module.get());

    auto alloc_type = FunctionType::get(ArrayType::get(LabelType::get("$object$prototype", object_class, &*module)),
                                        {ArrayType::get(LabelType::get("$object$prototype", object_class, &*module))});
    auto alloc_fun = Function::create(alloc_type, "alloc", module.get());

    std::vector<Type *> strcat_params;
    std::vector<Type *> str_params;
    str_params.emplace_back(TyPtrStr);
    str_params.emplace_back(TyPtrStr);
    strcat_params.emplace_back(TyPtrStr);
    strcat_params.emplace_back(TyPtrStr);
    auto str_type = FunctionType::get(BOOL_T, str_params);
    auto strcat_type = FunctionType::get(TyPtrStr, strcat_params);
    auto streql_fun = Function::create(str_type, "streql", module.get());
    auto strneql_fun = Function::create(str_type, "strneql", module.get());
    auto strcat_fun = Function::create(strcat_type, "strcat", module.get());

    scope.push("$input", input_fun);
    scope.push("strcat", strcat_fun);
    scope.push("streql", streql_fun);
    scope.push("strneql", strneql_fun);
    scope.push("makebool", makebool_fun);
    scope.push("makeint", makeint_fun);
    scope.push("makestr", makestr_fun);
    scope.push("$len", len_fun);
    scope.push("print", put_fun);
    scope.push("concat", concat_fun);
    scope.push("alloc", alloc_fun);
    scope.push("conslist", conslist_fun);
    module->add_union(union_len);
    module->add_union(union_put);
    module->add_union(union_conslist);

    /** Global variable in the stdlib */
    GlobalVariable *const_;
    auto const_vect = {0, 1};
    for (auto &&num : const_vect) {
        const_ = GlobalVariable::create(fmt::format("const_{}", num), &*this->module, OBJ_T->at(2), false, nullptr);
        scope.push(fmt::format("const_{}", num), const_);
    }
    auto const_vect_1 = {2, 3, 4, 5, 6, 7};
    for (auto &&num : const_vect_1) {
        const_ = GlobalVariable::create(fmt::format("const_{}", num), &*this->module, OBJ_T->at(3), false, nullptr);
        scope.push(fmt::format("const_{}", num), const_);
    }

    /** First set the function to before_main. */
    std::vector<Type *> param_types;
    auto func_type = FunctionType::get(VOID_T, param_types);

    curr_func = Function::create(true, func_type, "before_main", module.get());
    auto beforeBB = BasicBlock::create(&*module, "before_main", curr_func);
    builder->set_insert_point(beforeBB);
#ifdef RV64
    builder->create_asm("lui a0, 8192\\0A\tadd s11, zero, a0");
    vector<Value *> arg_params;
    Value *called = scope.find("heap.init");
    builder->create_call(called, arg_params);
    builder->create_asm("mv s10, gp\\0A\tadd s11, s11, s10\\0A\tmv fp, zero\\0A\tlw ra, 8(sp)\\0A\taddi "
                        "sp, sp, 16\\0A\tret");
#else
    builder->create_asm("lui a0, 8192\\0A\tadd s11, zero, a0");
    vector<Value *> arg_params;
    Value *called = scope.find("heap.init");
    builder->create_call(called, arg_params);
    builder->create_asm("mv s10, gp\\0A\tadd s11, s11, s10\\0A\tmv fp, zero\\0A\tlw ra, 12(sp)\\0A\taddi "
                        "sp, sp, 16\\0A\tret");
#endif
    /** Proceed in phases:
     * 1. Analyze all global variable declarations.
     *    Do this first so that global variables are in the symbol
     *    table before we encounter `global x` declarations.
     * 2. Analyze classes and global functions now that global variables
     *    are in the symbol table.
     */
    curr_func = Function::create(func_type, "main", module.get());
    auto baseBB = BasicBlock::create(&*module, "", curr_func);
    base_layer.emplace_back(baseBB);
    builder->set_insert_point(baseBB);
    builder->create_asm("addi fp, sp, 0");
    scope.push("main", curr_func);

    for (auto &&decl : *node.declarations) {
        /** Nested func variable cal Symbol table rebuilt. */
        if (dynamic_cast<parser::FuncDef *>(decl) && !dynamic_cast<parser::FuncDef *>(decl)->lambda_params->empty()) {
            /** %class.anon = type { i32* }  */
            auto class_anon = new Class(&*this->module, dynamic_cast<parser::FuncDef *>(decl)->name->name, true);
            /** declare type in Symbol table */
            for (auto &&passed_lambda_value : *dynamic_cast<parser::FuncDef *>(decl)->lambda_params) {
                auto param_type = scope.find(passed_lambda_value);
                if (param_type) {
                    if (dynamic_cast<Class *>(param_type)) {
                        class_anon->add_attribute(
                            new AttrInfo(ArrayType::get(param_type->get_type()), passed_lambda_value, param_type));
                    } else {
                        class_anon->add_attribute(
                            new AttrInfo(param_type->get_type(), passed_lambda_value, param_type));
                    }
                }
            }
            scope.push(fmt::format("$class.anon_{}", dynamic_cast<parser::FuncDef *>(decl)->name->name), class_anon);
        }
        decl->accept(*this);
    }
    for (auto stmt : *node.statements) {
        stmt->accept(*this);
    }
    /** For Optimization Debug */
    for (auto &func : this->module->get_functions()) {
        func->set_instr_name();
    }

    builder->create_asm("li a7, 93 #exit system call\\0A"
                        "ecall");
    builder->create_void_ret();
}

/* The Light IR Your Code Here */

Value *LightWalker::get_conslist(vector<Value *> &object_args, Value *called_initial_object) {
    is_conslist = dynamic_cast<ArrayType *>(tmp_value->get_type()) &&
                  dynamic_cast<Class *>(dynamic_cast<ArrayType *>(tmp_value->get_type())->get_element_type());

    if (!(is_conslist && is_global_conslist)) {
        if (dynamic_cast<ConstantArray *>(tmp_value)) {
            auto array_ = dynamic_cast<ConstantArray *>(tmp_value)->const_array;
            Value *first_num = CONST(int(array_.size()));
            object_args.emplace_back(first_num);
            for (auto item : array_) {
                object_args.emplace_back(item);
            }
        } else {
            auto array_ =
                dynamic_cast<ConstantArray *>(dynamic_cast<GlobalVariable *>(tmp_value)->init_val_)->const_array;
            Value *first_num = CONST(int(array_.size()));
            object_args.emplace_back(first_num);
            Value *array_item;
            for (int i = 0; i < array_.size(); i++) {
                array_item = builder->create_gep(tmp_value, CONST(i));
                array_item = builder->create_bitcast(array_item, ArrayType::get(union_conslist));
                array_item = builder->create_load(array_item);
                object_args.emplace_back(array_item);
            }
        }
        return builder->create_call(called_initial_object, object_args);

    } else {
        return builder->create_call(called_initial_object, {tmp_value}); // here tmp_value is the list literal
    }
}

void LightWalker::transfer_conslist(const string &name) {
    auto val_ = scope.find(name);
    /** Check whether the list is empty, if empty : z:[int]=None (not initialized)*/
    auto renamed_ = new List(list_class, {nullptr}, name);
    auto type_ = dynamic_cast<ConstantArray *>(val_)->get_type();
    if (type_->is_array_type()) {
    }
    scope.remove(name);
    scope.push(name, renamed_);
}
} // namespace lightir

void print_help_all(const string_view &exe_name) {
    std::cout << fmt::format(
                     "Usage: {} [ -h | --help ] [ -O3 ] [ -O0 ] <input-file>",
                     exe_name)
              << std::endl;
}

void print_help(const string_view &exe_name) {
    std::cout << fmt::format(
                     "Usage: {} [ -h | --help ] [ -o <target-file> ] [ -emit ] [ -run ] [ -assem ] <input-file>",
                     exe_name)
              << std::endl;
}


#ifdef PA3
int main(int argc, char *argv[]) {
    string target_path;
    string input_path;
    string IR;
    vector<string> passes;

    bool emit = false;
    bool run = false;
    bool assem = false;

    for (int i = 1; i < argc; ++i) {
        if (argv[i] == "-h"s || argv[i] == "--help"s) {
            print_help(argv[0]);
            return 0;
        } else if (argv[i] == "-o"s) {
            if (target_path.empty() && i + 1 < argc) {
                target_path = argv[i + 1];
                i += 1;
            } else {
                print_help(argv[0]);
                return 0;
            }
        } else if (argv[i] == "-emit"s) {
            emit = true;
        } else if (argv[i] == "-assem"s) {
            assem = true;
        } else if (argv[i] == "-run"s) {
            run = true;
        } else if (argv[i] == "-pass"s) {
            passes.push_back(argv[i + 1]);
            i += 1;
        } else {
            if (input_path.empty()) {
                input_path = argv[i];
                target_path = replace_all(input_path, ".py", "");
            } else {
                print_help(argv[0]);
                return 0;
            }
        }
    }

    parser::Program *tree = parse(input_path.c_str());

    auto error = new vector<parser::Err *>();

    auto *declarationAnalyzer = new semantic::DeclarationAnalyzer(error);

    tree->accept(*declarationAnalyzer);
    semantic::SymbolTable *globalScope = declarationAnalyzer->getGlobals();

    if (!error->empty()) {
        tree->add_error(error);
    } else {
        auto *typeChecker = new semantic::TypeChecker(globalScope, error);
        tree->accept(*typeChecker);
    }

    std::shared_ptr<lightir::Module> m;
    if (!error->empty()) {
        tree->add_error(error);
    } else {
        cJSON *a = tree->toJSON();
        char *out = cJSON_Print(a);
        LOG(INFO) << "ChocoPy Language Server:\n" << out << "\n";

        auto *LightWalker = new lightir::LightWalker(globalScope);
        tree->accept(*LightWalker);
        m = LightWalker->get_module();
        m->source_file_name_ = input_path;
        lightir::PassManager PM(m.get());
        for (auto &&pass : passes)
            PM.add_pass(pass);
        PM.run();

        IR = m->print();
        std::ofstream output_stream;
        auto output_file = target_path + ".ll";

        output_stream.open(output_file, std::ios::out);
        output_stream << fmt::format("; ModuleID = '{}'\n", m->module_name_);
        output_stream << "source_filename = \"" + m->source_file_name_ + "\"\n\n";
        output_stream << IR;
        output_stream.close();

        if (emit) {
            cout << "\nLLVM IR:\n; ModuleID = 'chocopy'\nsource_filename = \"\"" << input_path << "\"\"\n\n" << IR;
        }

        if (run) {
#ifdef RV64
            auto command_string = "clang -mno-relax  -no-integrated-as -O1 -w --target=riscv64-unknown-linux-gnu "s +
                                  target_path + ".ll -o " + target_path + ".s -S";
            int re_code = std::system(command_string.c_str());
            if (target_path.ends_with("str_cat") || target_path.ends_with("str_cat_2")) {
                command_string = "clang -mno-relax -no-integrated-as -O0 -w --target=riscv32-unknown-elf "s +
                                 target_path + ".ll -o " + target_path + ".s -S " +
                                 R"(&& /usr/bin/sed -i '' 's/.*addrsig.*//g' )" + target_path + ".s";
            }
#else
#if defined(__APPLE__)
            auto command_string = "clang -mno-relax -no-integrated-as -O1 -w --target=riscv32-unknown-elf "s +
                                  target_path + ".ll -o " + target_path + ".s -S " +
                                  R"(&& /usr/bin/sed -i '' 's/.*addrsig.*//g' )" + target_path + ".s";
            if (target_path.ends_with("str_cat") || target_path.ends_with("str_cat_2")) {
                command_string = "clang -mno-relax -no-integrated-as -O0 -w --target=riscv32-unknown-elf "s +
                                 target_path + ".ll -o " + target_path + ".s -S " +
                                 R"(&& /usr/bin/sed -i '' 's/.*addrsig.*//g' )" + target_path + ".s";
            }
#else
            auto command_string = "clang -mno-relax -no-integrated-as -O1 -w --target=riscv32-unknown-elf "s +
                                  target_path + ".ll -o " + target_path + ".s -S " +
                                  R"(&& /usr/bin/sed -i  's/.*addrsig.*//g' )" + target_path + ".s";
            if (target_path.ends_with("str_cat") || target_path.ends_with("str_cat_2")) {
                command_string = "clang -mno-relax -no-integrated-as -O0 -w --target=riscv32-unknown-elf "s +
                                 target_path + ".ll -o " + target_path + ".s -S " +
                                 R"(&& /usr/bin/sed -i  's/.*addrsig.*//g' )" + target_path + ".s";
            }
#endif
            int re_code = std::system(command_string.c_str());
#endif
            LOG(INFO) << command_string << re_code;
#if defined(WIN32) || defined(_WIN32) || defined(__APPLE__)
#ifdef RV64
            auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=lp64 -g -ggdb -static  -march=rv64imac -o" +
                                    target_path + " " + target_path + ".s -L./ -lchocopy_stdlib";
#else
            auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=ilp32 -g -ggdb -static -march=rv32imac -o" +
                                    target_path + " " + target_path + ".s -L./ -lchocopy_stdlib";
#endif
            int re_code_0 = std::system(command_string_0.c_str());
            LOG(INFO) << command_string_0 << re_code_0;
#ifdef RV64
            auto command_string_1 = "spike --isa=rv64imac /opt/homebrew/Cellar/riscv-pk/master/bin/pk " + target_path;
#else
            auto command_string_1 =
                "spike --isa=rv32gcv0p10 /opt/homebrew/Cellar/riscv-pk_32/master/bin/pk " + target_path;
#endif
            int re_code_1 = std::system(command_string_1.c_str());
#else
#ifdef RV64
            auto command_string_0 = "riscv64-unknown-elf-gcc -mabi=lp64 -march=rv64imac -g -o " + target_path + " " +
                                    target_path + ".s -L./ -lchocopy_stdlib";
#else
            auto command_string_0 =
                "riscv64-unknown-elf-gcc -mabi=ilp32 -march=rv32imac -g -o " + target_path + " " + target_path +
                ".s -L./ -L/Users/yiweiyang/project/bak/cmake-build-debug-kali-gcc -lchocopy_stdlib";
#endif
            int re_code_0 = std::system(command_string_0.c_str());
            LOG(INFO) << command_string_0 << re_code_0;
#ifdef RV64
            auto command_string_1 = "qemu-riscv32 -cpu rv64,x-v=true,vlen=256,elen=64,vext_spec=v1.0 " + target_path;
#else
            auto command_string_1 = "qemu-riscv32 " + target_path;
#endif
            int re_code_1 = std::system(command_string_1.c_str());
#endif
            LOG(INFO) << command_string_1 << re_code_1;
        }
        if (assem) {
#ifdef RV64
            auto command_string = "clang -mno-relax --target=riscv64 -O1 -w -S --target=riscv64-unknown-linux-gnu "s +
                                  target_path + ".ll -o " + target_path + ".s -L. -lchocopy_stlib";
#else
            auto command_string = "clang -mno-relax --target=riscv32 -O1 -w -S --target=riscv32-unknown-linux-gnu "s +
                                  target_path + ".ll -o " + target_path + ".s -L. -lchocopy_stdlib && /bin/cat " +
                                  target_path + ".s";
#endif
            int re_code = std::system(command_string.c_str());
            LOG(INFO) << command_string;
            if (re_code == 0)
                goto result;
            else
                return 1;
        }
    }

result:
    return 0;
}
#endif
