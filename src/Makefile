#该 Makefile 旨在编译该目录下的所有 .c 文件，并在 ../lib/ 目录下形成 tlpi_hdr.so 动态库文件

# 指定编译器为 gcc
COMPLIER = gcc

# 通过 wildcard 函数取出当前目录下所有的 .c 文件并赋值给 SRC
SRC      = $(wildcard *.c)

#指定中间文件 (.o) 的目录
OBJ_DIR  = ../lib

#指定编译完成后的库名以及路径
LIB_NAME = ../lib/tlpi_hdr.so

# 通过 patsubst 函数，将所有通过 notdir 函数去除路径名的源文件编译成目标文件放到 OBJ_DIR 所在的目录下
OBJ		 = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC)))

# 指定编译器选项
# -fPIC 全称为 Position Independent Code，意为生成与位置无关的代码，这对编译库文件很重要，因为动态连接库会在进程任意地址加载
# -g 生成调试文件，供 gdb 使用
# -Wall 显示所有的警告（Warning），诸如 未使用临时变量，每检查函数返回值等
# -O3 开启最大优化
COMPLIER_OPTIONS = -fPIC -g -Wall -O3

$(OBJ_DIR)/%.o : %.c # 每一个 OBJ_DIR 目录下的 .o 文件都依赖一个 .c 文件 
# $< 表示指定源文件 
# $@ 表示目标文件 
# -c 编译器选项表示只编译但不链接
	$(COMPLIER) $(COMPLIER_OPTIONS) -c $< -o $@

# 最后将所有的中间文件合并成一个动态库文件 (.so)
$(LIB_NAME) : $(OBJ)
	$(COMPLIER) -shared -o $@ $(OBJ)

.PHONY: clean
clean: # 可以调用 make clean 来清理整个编译过程中的中间文件
	rm -f $(OBJ_DIR)/*.o