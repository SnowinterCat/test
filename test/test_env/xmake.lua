-- target("test_env")
--     set_kind("binary")
--     set_targetdir("$(testdir)")

--     add_deps("base")
--     add_packages("spdlog")

--     -- add_includedirs("include", {public = true})
--     -- add_headerfiles("include/(**)")
--     -- add_headerfiles("src/(**.hpp)", {install = false})
--     add_files("src/**.cpp")

--     after_build(function (target) 
--         import("lua.auto", {rootdir = os.projectdir()})
--         auto().target_autoclean(target)
--     end)

--     -- add_runenvs("LD_LIBRARY_PATH", ".")
-- target_end()