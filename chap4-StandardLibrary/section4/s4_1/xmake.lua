add_rules("mode.debug")
set_toolchains("@llvm")

add_requires("boost")

target("p1") do
    set_kind("binary")
    add_files("./p1.cxx")
    -- add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end