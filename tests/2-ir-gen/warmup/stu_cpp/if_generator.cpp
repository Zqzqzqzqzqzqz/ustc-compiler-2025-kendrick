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
// 定义一个从常数值获取/创建 ConstantFP 类实例化的宏，方便多次调用
#define CONST_FP(num) ConstantFP::get(num, module)

/*
    目标C代码：
    int main() {
        float a = 5.555;
        if (a > 1)
            return 233;
        return 0;
    }
*/
int main() {
  // 创建一个 Module 实例
  auto module = new Module();
  // 创建一个 IRBuilder 实例
  auto builder = new IRBuilder(nullptr, module);
  // 从 Module 处获取所需类型
  Type *Int32Type = module->get_int32_type();
  Type *FloatType = module->get_float_type();

  // 创建 main 函数
  auto mainFunTy = FunctionType::get(Int32Type, {});
  auto mainFun = Function::create(mainFunTy, "main", module);

  // 创建 main 函数的入口 BasicBlock
  auto bb = BasicBlock::create(module, "entry", mainFun);
  builder->set_insert_point(bb);

  // float a = 5.555;
  // 在栈上为 a 分配空间
  auto aAlloca = builder->create_alloca(FloatType);
  // 将 5.555 存入 a
  builder->create_store(CONST_FP(5.555), aAlloca);

  // if (a > 1)
  // 加载 a 的值
  auto aLoad = builder->create_load(aAlloca);
  // 比较 a > 1.0
  auto fcmp = builder->create_fcmp_gt(aLoad, CONST_FP(1.0));
  // 创建 true 和 false 分支的 BasicBlock
  auto truebb = BasicBlock::create(module, "true_br", mainFun);
  auto falsebb = BasicBlock::create(module, "false_br", mainFun);

  // 创建条件跳转指令
  builder->create_cond_br(fcmp, truebb, falsebb);

  // true 分支
  builder->set_insert_point(truebb);
  // return 233;
  builder->create_ret(CONST_INT(233));

  // false 分支
  builder->set_insert_point(falsebb);
  // return 0;
  builder->create_ret(CONST_INT(0));

  // 输出 module 中的所有 IR 指令
  std::cout << module->print();
  delete module;
  return 0;
}