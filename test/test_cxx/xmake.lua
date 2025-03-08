target("test_cxx")
    set_kind("binary")
    set_targetdir("$(testdir)")

    -- add_deps("base")
    add_rules("target.autoclean")
    add_packages("spdlog")

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)")
    -- add_headerfiles("src/(**.hpp)", {install = false})
    add_files("src/**.cpp")
target_end()