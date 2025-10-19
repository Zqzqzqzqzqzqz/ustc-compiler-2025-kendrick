define i32 @main() {
entry:
%a = alloca [10 x i32]
%a0_ptr = getelementptr [10 x i32], [10 x i32]* %a, i32 0, i32 0
store i32 10, i32* %a0_ptr
%a0_ptr_reload = getelementptr [10 x i32], [10 x i32]* %a, i32 0, i32 0
%a0_val = load i32, i32* %a0_ptr_reload
%mul_val = mul i32 %a0_val, 2
%a1_ptr = getelementptr [10 x i32], [10 x i32]* %a, i32 0, i32 1
store i32 %mul_val, i32* %a1_ptr
%a1_ptr_reload = getelementptr [10 x i32], [10 x i32]* %a, i32 0, i32 1
%ret_val = load i32, i32* %a1_ptr_reload
ret i32 %ret_val
}