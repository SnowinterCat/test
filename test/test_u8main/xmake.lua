target("test_u8main")
    set_kind(is_config("use_luancher", true) and "shared" or "binary")
    set_targetdir("$(testdir)")

    add_deps("luancher")
    add_packages("spdlog")

    -- add_includedirs("include", {public = true})
    -- add_headerfiles("include/(**)")
    -- add_headerfiles("src/(**.hpp)", {install = false})
    add_files("src/**.cpp")

    after_build(function (target) 
        import("lua.auto", {rootdir = os.projectdir()})
        auto().target_autoclean(target)
        auto().binary_autoluanch(target)
    end)
target_end()