# OpenGL Learning

## GLSL(OpenGL 着色器语言)

### 典型着色器程序结构：
```GLSL
#version version_number
//输入
in type in_variable_name;
in type in_variable_name;
//输出
out type out_variable_name;
uniform type uniform_name;
//主函数
int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```
### GLSL 数据类型
