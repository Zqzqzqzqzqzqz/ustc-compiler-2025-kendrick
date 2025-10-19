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
        int a;
        int i;
        a = 10;
        i = 0;
        while (i < 10) {
            i = i + 1;
            a = a + i;
        }
        return a;
    }
*/
int main() {
  // 创建一个 Module 实例
  auto module = new Module();
  // 创建一个 IRBuilder 实例
  auto builder = new IRBuilder(nullptr, module);
  // 从 Module 处获取所需类型
  Type *Int32Type = module->get_int32_type();

  // 创建 main 函数
  auto mainFunTy = FunctionType::get(Int32Type, {});
  auto mainFun = Function::create(mainFunTy, "main", module);
  // 创建 main 函数的入口 BasicBlock
  auto entryBB = BasicBlock::create(module, "entry", mainFun);
  builder->set_insert_point(entryBB);

  // int a; int i;
  // 为 a 和 i 分配栈空间
  auto aAlloca = builder->create_alloca(Int32Type);
  auto iAlloca = builder->create_alloca(Int32Type);

  // a = 10; i = 0;
  builder->create_store(CONST_INT(10), aAlloca);
  builder->create_store(CONST_INT(0), iAlloca);

  // 创建循环判断、循环体和循环结束的 BasicBlock
  auto condbb = BasicBlock::create(module, "cond", mainFun);
  auto bodybb = BasicBlock::create(module, "body", mainFun);
  auto endbb = BasicBlock::create(module, "end", mainFun);
  // 从入口块无条件跳转到循环判断块
  builder->create_br(condbb);

  // 循环判断块
  builder->set_insert_point(condbb);
  // 加载 i 的当前值
  auto iLoad = builder->create_load(iAlloca);
  // 比较 i < 10
  auto icmp = builder->create_icmp_lt(iLoad, CONST_INT(10));
  // 根据比较结果，跳转到循环体或循环结束块
  builder->create_cond_br(icmp, bodybb, endbb);
  // 循环体块
  builder->set_insert_point(bodybb);
  // i = i + 1;
  iLoad = builder->create_load(iAlloca);
  auto iAdd = builder->create_iadd(iLoad, CONST_INT(1));
  builder->create_store(iAdd, iAlloca);
  // a = a + i;
  auto aLoad = builder->create_load(aAlloca);
  iLoad = builder->create_load(iAlloca); // 重新加载更新后的 i
  auto aAdd = builder->create_iadd(aLoad, iLoad);
  builder->create_store(aAdd, aAlloca);
  // 无条件跳转回循环判断块
  builder->create_br(condbb);

  // 循环结束块
  builder->set_insert_point(endbb);
  // 加载 a 的最终值
  aLoad = builder->create_load(aAlloca);
  // return a;
  builder->create_ret(aLoad);

  // 输出 module 中的所有 IR 指令
  std::cout << module->print();
  delete module;
  return 0;
}
