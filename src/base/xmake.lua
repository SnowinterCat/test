target("base")
    set_kind("$(kind)")
    set_targetdir("$(libdir)")

    -- add_deps("base")
    add_rules("target.autoclean", "target.autoname", "library.autodefine")
    -- add_packages("spdlog")

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)", "include/(**.h)")
    -- add_headerfiles("src/(**.hpp)", {install = false})
    add_files("src/**.cpp")

    set_configdir("include/test/base")
    add_configfiles("config.h.in", {public = true})
target_end()