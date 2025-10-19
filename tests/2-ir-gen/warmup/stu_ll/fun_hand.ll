define i32 @callee(i32 %a) {
entry:
%a_addr = alloca i32
store i32 %a, i32* %a_addr
%tmp = load i32, i32* %a_addr
%mul_val = mul i32 2, %tmp
ret i32 %mul_val
}

define i32 @main() {
entry:
%call_ret = call i32 @callee(i32 110)
ret i32 %call_ret
}