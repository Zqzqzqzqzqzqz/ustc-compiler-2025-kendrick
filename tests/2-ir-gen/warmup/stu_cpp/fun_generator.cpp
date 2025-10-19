#include "BasicBlock.hpp"
#include "Constant.hpp"
#include "Function.hpp"
#include "IRBuilder.hpp"
#include "Module.hpp"
#include "Type.hpp"

#include <iostream>
#include <memory>

// 定义一个从常数值获取/创建 ConstantInt 类实例化的宏，方便多次调用
#define CONST_INT(num) ConstantInt::get(num, module)

/*
    目标C代码：
    int callee(int a) { return 2 * a; }
    int main() { return callee(110); }
*/

int main() {
  // 创建一个 Module 实例
  auto module = new Module();
  // 创建一个 IRBuilder 实例
  auto builder = new IRBuilder(nullptr, module);
  // 从 Module 处获取32位整形类型
  Type *Int32Type = module->get_int32_type();
  // 创建 callee 函数
  // 声明函数参数类型
  std::vector<Type *> callee_params;
  callee_params.push_back(Int32Type);
  // 创建函数类型
  auto calleeFunTy = FunctionType::get(Int32Type, callee_params);
  // 创建函数
  auto calleeFun = Function::create(calleeFunTy, "callee", module);
  // 创建 callee 函数的入口 BasicBlock
  auto bb = BasicBlock::create(module, "entry", calleeFun);
  builder->set_insert_point(bb);

  // 获取函数参数
  auto arg_a = calleeFun->get_args().begin();
  // return 2 * a;
  auto mul = builder->create_imul(CONST_INT(2), &(*arg_a));
  builder->create_ret(mul);

  // 创建 main 函数
  auto mainFunTy = FunctionType::get(Int32Type, {});
  auto mainFun = Function::create(mainFunTy, "main", module);

  // 创建 main 函数的入口 BasicBlock
  bb = BasicBlock::create(module, "entry", mainFun);
  builder->set_insert_point(bb);
  // 创建 call 指令
  auto call = builder->create_call(calleeFun, {CONST_INT(110)});
  // 返回 call 的结果
  builder->create_ret(call);

  // 输出 module 中的所有 IR 指令
  std::cout << module->print();
  delete module;
  return 0;
}