target("test_vulkan")
    set_kind("shared")
    set_targetdir("$(testdir)")

    add_deps("luancher", "vulkan", "sdl3")
    add_packages("spdlog", "imgui")

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