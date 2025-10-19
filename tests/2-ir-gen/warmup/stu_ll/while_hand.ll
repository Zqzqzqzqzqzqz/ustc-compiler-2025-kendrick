define i32 @main() {
entry:
%a = alloca i32
%i = alloca i32
store i32 10, i32* %a
store i32 0, i32* %i
br label %loop_cond

loop_cond:
%i_val = load i32, i32* %i
%cmp = icmp slt i32 %i_val, 10
br i1 %cmp, label %loop_body, label %loop_end

loop_body:
%i_val1 = load i32, i32* %i
%i_new = add i32 %i_val1, 1
store i32 %i_new, i32* %i
%a_val = load i32, i32* %a
%i_val2 = load i32, i32* %i
%a_new = add i32 %a_val, %i_val2
store i32 %a_new, i32* %a
br label %loop_cond

loop_end:
%ret_val = load i32, i32* %a
ret i32 %ret_val
}