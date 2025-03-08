-- 工程设置
set_project("test")
set_version("0.0.1", {build = "$(buildversion)"})

-- 全局设置
set_warnings("allextra")
set_languages("cxx20", "c17")
set_exceptions("cxx")
set_encodings("utf-8")

-- 添加编译选项
add_rules("mode.release", "mode.debug", "mode.releasedbg", "mode.minsizerel")
add_rules("plugin.compile_commands.autoupdate", {lsp = "clangd", outputdir = ".vscode"})

-- 编译设置
option("3rd_kind",     {showmenu = true, default = "shared", values = {"static", "shared"}})
option("buildversion", {showmenu = true, default = "0", type = "string"})

option("outputdir")
    set_showmenu(true)
    on_check(function (option)
        if not option:value() then
            option:set_value(path.translate(vformat("$(projectdir)/bin", xmake)))
        end
    end)
option_end()

-- 编译规则
rule("target.autoclean")
    after_build(function (target)
        os.tryrm(target:targetdir() .. "/" .. target:basename() .. ".exp")
        os.tryrm(target:targetdir() .. "/" .. target:basename() .. ".ilk")
        os.tryrm(target:targetdir() .. "/compile." .. target:basename() .. ".pdb")
    end)
rule_end()

rule("target.autoname")
    on_load(function (target)
        function Camel(str)
            return str:sub(1, 1):upper() .. str:sub(2)
        end
        import("core.project.project")
        target:set("basename", Camel(project:name()) .. Camel(target:name()))
    end)
rule_end()

rule("library.autodefine")
    before_build(function (target)
        import("core.project.project")
        local projNameUpper = string.upper(project.name())
        local targNameUpper = string.upper(target:name())
        if target:kind() == "shared" then
            target:add("defines", projNameUpper .. "_" .. targNameUpper .. "_DLL")
        end
        target:add("defines", projNameUpper .. "_" .. targNameUpper .. "_EXPORTS")
    end)
rule_end()

-- 隐藏设置与规则
includes("lua/hideoptions.lua")
includes("lua/hiderules.lua")
includes("lua/hidetargets.lua")

-- 第三方库依赖
-- some third-libraries use our own configurations
add_repositories("myrepo 3rd", {rootdir = os.scriptdir()})
-- header-only libraries
add_requires(
    -- tools
    -- "cxxopts",
    -- "rapidjson",
    -- c++23
    "out_ptr"
)
-- normal libraries
-- add_requires("libsdl3", {override = true, version = "3.x.x", configs = {shared = is_config("3rd_kind", "shared")}})
add_requires("spdlog", {override = true, version = "1.x.x", configs = {shared = is_config("3rd_kind", "shared"), header_only = false, fmt_external = true, wchar = true, wchar_console = true}})
-- normal libraries' dependencies configurations
add_requireconfs("**.fmt", {override = true, version = "11.0.x", configs = {shared = is_config("3rd_kind", "shared"), header_only = false}})

includes("src/*/xmake.lua")
includes("exec/*/xmake.lua")
includes("test/*/xmake.lua")
