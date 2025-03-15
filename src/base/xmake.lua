target("base")
    set_kind("headeronly")

    add_includedirs("include", {public = true})
    add_headerfiles("include/(**)")
    -- add_headerfiles("src/(**.hpp)", {install = false})
    -- add_files("src/**.cpp")

    set_configdir("include/test")
    set_configvar("USE_LUANCHER", is_config("use_luancher", true) and "true" or "false", {quote = false})
    add_configfiles("config.h.in", {public = true})
target_end()