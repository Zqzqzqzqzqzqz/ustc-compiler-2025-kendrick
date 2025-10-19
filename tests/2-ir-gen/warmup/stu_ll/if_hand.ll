define i32 @main() {
entry:
%a = alloca float
store float 5.555000e+00, float* %a
%a_val = load float, float* %a
%cmp = fcmp ugt float %a_val, 1.000000e+00
br i1 %cmp, label %if_true, label %if_false

if_true:
ret i32 233

if_false:
ret i32 0
}