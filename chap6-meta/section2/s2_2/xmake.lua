add_rules("mode.debug")
set_toolchains("@llvm")

-- add_requires("boost")

target("p1") do
    set_kind("binary")
    add_files("./p1.cxx")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("p2") do
    set_kind("binary")
    add_files("./p2.cxx")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("p3") do
    set_kind("binary")
    add_files("./p3.cxx")
	set_languages("cxx17")
	set_targetdir("./build")
end
