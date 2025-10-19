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
        int a[10];
        a[0] = 10;
        a[1] = a[0] * 2;
        return a[1];
    }
*/
int main() {
  // 创建一个 Module 实例
  auto module = new Module();
  // 创建一个 IRBuilder 实例
  auto builder = new IRBuilder(nullptr, module);
  // 从 Module 处获取32位整形和数组类型
  Type *Int32Type = module->get_int32_type();
  auto *ArrayType_a = ArrayType::get(Int32Type, 10);
  // 创建 main 函数
  auto mainFunTy = FunctionType::get(Int32Type, {});
  auto mainFun = Function::create(mainFunTy, "main", module);

  // 创建 main 函数的入口 BasicBlock
  auto bb = BasicBlock::create(module, "entry", mainFun);
  builder->set_insert_point(bb);

  // 在栈上为数组 a 分配空间
  // int a[10];
  auto aAlloca = builder->create_alloca(ArrayType_a);
  // a[0] = 10;
  // 获取 a[0] 的地址
  auto a0GEP = builder->create_gep(aAlloca, {CONST_INT(0), CONST_INT(0)});
  // 将常数 10 存入 a[0]
  builder->create_store(CONST_INT(10), a0GEP);
  // a[1] = a[0] * 2;
  // 再次获取 a[0] 的地址并加载其值
  auto a0GEP_load = builder->create_gep(aAlloca, {CONST_INT(0), CONST_INT(0)});
  auto a0Load = builder->create_load(a0GEP_load);
  // 计算 a[0] * 2
  auto mul = builder->create_imul(a0Load, CONST_INT(2));
  // 获取 a[1] 的地址
  auto a1GEP = builder->create_gep(aAlloca, {CONST_INT(0), CONST_INT(1)});
  // 将计算结果存入 a[1]
  builder->create_store(mul, a1GEP);

  // 加载 a[1] 的值
  auto a1GEP_load = builder->create_gep(aAlloca, {CONST_INT(0), CONST_INT(1)});
  auto a1Load = builder->create_load(a1GEP_load);
  // 返回 a[1] 的值
  builder->create_ret(a1Load);

  // 输出 module 中的所有 IR 指令
  std::cout << module->print();
  delete module;
  return 0;
}