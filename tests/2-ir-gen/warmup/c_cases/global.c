int g_var = 10; // 定义一个全局整型变量并初始化
int g_arr[3];   // 定义一个全局整型数组，默认初始化为0

int main(void) {
  int temp;
  g_arr[0] = 5;            // 修改全局数组的元素
  temp = g_var + g_arr[0]; // 读取全局变量和全局数组
  g_var = temp;            // 修改全局变量
  return g_var;
}