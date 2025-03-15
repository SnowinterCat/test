target("base")
    set_kind("headeronly")

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**.hpp)", "include/(**.h)")
    -- add_headerfiles("src/(**.hpp)", {install = false})
    -- add_files("src/**.cpp")

    set_configdir("include/test")
    add_configfiles("config.h.in", {public = true})
target_end()