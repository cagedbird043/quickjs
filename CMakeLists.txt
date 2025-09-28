# ===================================================================
# CMake Build Script for QuickJS as an Independent Library
# Target: libquickjs.a for HarmonyOS NEXT (aarch64-linux-ohos)
# ===================================================================
cmake_minimum_required(VERSION 3.10)

project(QuickJS_Standalone_Build C)

# --- 读取 QuickJS 版本号，用于编译宏 ---
# 这会读取 VERSION 文件的内容并将其作为编译定义。
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" QJS_VERSION)
string(STRIP "${QJS_VERSION}" QJS_VERSION) # 移除末尾的换行符

# --- 定义构建静态库所需的核心源文件 ---
# 这些是构成 libquickjs.a 的所有必要部分。
set(QUICKJS_LIB_SOURCES
    quickjs.c
    libregexp.c
    libunicode.c
    cutils.c
    quickjs-libc.c  # 包含 std 和 os 模块的 C 实现
    dtoa.c
)

# --- 创建静态库目标 "quickjs_static" ---
add_library(quickjs_static STATIC ${QUICKJS_LIB_SOURCES})

# --- 设定最终输出的文件名为 "libquickjs.a" ---
# 这可以确保无论在什么平台下编译，我们得到的都是标准的文件名。
set_target_properties(quickjs_static PROPERTIES 
    OUTPUT_NAME "quickjs"
)

# --- 添加必要的编译宏定义 ---
target_compile_definitions(quickjs_static PRIVATE
    _GNU_SOURCE
    "CONFIG_VERSION=\"${QJS_VERSION}\""
    # 关键：禁用 BIGNUM 以获得最小化核心引擎，并移除对 libbf 库的依赖。
    "CONFIG_BIGNUM=0" 
)

# --- 添加必要的编译选项 ---
# -fPIC (位置无关代码) 是在现代操作系统上创建可链接库的强制要求。
target_compile_options(quickjs_static PRIVATE -fPIC)

# --- 公开头文件目录 ---
# 这是一个良好的实践，确保任何链接此库的目标都能找到头文件。
target_include_directories(quickjs_static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- 构建完成后的提示信息 ---
message(STATUS "QuickJS configured for HarmonyOS (aarch64-linux-ohos).")
message(STATUS "Run 'ninja' to build the target: libquickjs.a")