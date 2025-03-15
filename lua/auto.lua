local autofunc = autofunc or {}

import("core.project.project")

-- private

function _Camel(str)
    return str:sub(1, 1):upper() .. str:sub(2)
end

-- public

function autofunc.target_autoclean(target)
    os.tryrm(target:targetdir() .. "/" .. target:basename() .. ".exp")
    os.tryrm(target:targetdir() .. "/" .. target:basename() .. ".ilk")
    os.tryrm(target:targetdir() .. "/compile." .. target:basename() .. ".pdb")
end

function autofunc.target_autoname(target)
    target:set("basename", _Camel(project:name()) .. _Camel(target:name()))
end

function autofunc.library_autodefine(target)
    local projNameUpper = string.upper(project:name())
    local targNameUpper = string.upper(target:name())
    if target:kind() == "shared" then
        target:add("defines", projNameUpper .. "_" .. targNameUpper .. "_DLL")
    end
    target:add("defines", projNameUpper .. "_" .. targNameUpper .. "_EXPORTS")
end

function autofunc.binary_autoluanch(target)
    print(target:name() .. " auto luanch")
end

function main()
    return autofunc
end