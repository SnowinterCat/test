-- 工程设置
set_project("test")
set_version("0.0.1", {build = "$(buildversion)"})

-- 全局设置
option("stdc",   {showmenu = true, default = 23, values = {23, 17}})
option("stdcxx", {showmenu = true, default = 23, values = {23, 20, 17}})
function stdc_ver() 
    return get_config("stdc") and "c" .. get_config("stdc") or nil
end
function stdcxx_ver() 
    return get_config("stdcxx") and "c++" .. get_config("stdcxx") or nil
end

set_warnings("allextra")
set_languages(stdc_ver(), stdcxx_ver())
set_exceptions("cxx")
set_encodings("utf-8")
set_policy("package.cmake_generator.ninja", true)

-- 添加编译选项
add_rules("mode.release", "mode.debug", "mode.releasedbg", "mode.minsizerel")
add_rules("plugin.compile_commands.autoupdate", {lsp = "clangd", outputdir = ".vscode"})

-- 编译设置
option("3rd_kind",     {showmenu = true, default = "shared", values = {"static", "shared"}})
option("buildversion", {showmenu = true, default = "0", type = "string"})
option("outputdir",    {showmenu = true, default = path.join(os.projectdir(), "bin"), type = "string"})
option("luanch",       {showmenu = true, default = nil, type = "string"})   -- 调试时，设置此值为要调试的组件名即可

includes("@builtin/check")
check_macros("has_std_out_ptr",  "__cpp_lib_out_ptr",  {name = "has_std_out_ptr", languages = stdcxx_ver(), includes = "memory"})
check_macros("has_std_expected", "__cpp_lib_expected", {name = "has_std_expected", languages = stdcxx_ver(), includes = "expected"})

-- 隐藏设置、隐藏目标、打包命令
includes("lua/hideoptions.lua")
includes("lua/hidetargets.lua")
includes("lua/pack.lua")

-- 第三方库依赖
-- some third-libraries use our own configurations
add_repositories("myrepo 3rd", {rootdir = os.scriptdir()})
-- header-only libraries
if not get_config("has_std_out_ptr") then
    add_requires("out_ptr")
end
if not get_config("has_std_expected") then
    add_requires("tl_expected")
end
add_requires("cxxopts", {version = "3.x.x"})
add_requires("vulkan-memory-allocator-hpp", {version = "3.1.0"})
add_requires("tinygltf", {version = "x.x.x"})
-- normal libraries
add_requires("vulkansdk")
add_requires("libsdl3", {version = "3.x.x", configs = {shared = is_config("3rd_kind", "shared"), x11 = true, wayland = true}})
add_requires("imgui", {version = "1.x.x", configs = {shared = is_config("3rd_kind", "shared")}})
add_requires("spdlog", {version = "1.x.x", configs = {shared = is_config("3rd_kind", "shared"), header_only = false, fmt_external = true, wchar = true, wchar_console = true}})
-- normal libraries' dependencies configurations
add_requireconfs("**.vulkan-memory-allocator", {override = true, version = "3.1.0"})
add_requireconfs("**.vulkan-headers", {override = true, version = "1.3.296"})
add_requireconfs("**.libxext", {system = true})     -- from libsdl3
add_requireconfs("**.libx11", {system = true})      -- from libsdl3
add_requireconfs("**.wayland", {system = true})     -- from libsdl3
add_requireconfs("**.fmt", {override = true, version = "11.0.x", configs = {shared = is_config("3rd_kind", "shared"), header_only = false}})
add_requireconfs("**.nlohmann_json", {override = true, version = "3.x.x"})
add_requireconfs("**.stb", {override = true, version = "x.x.x"})

includes("src/*/xmake.lua")
includes("exec/*/xmake.lua")
includes("test/*/xmake.lua")
