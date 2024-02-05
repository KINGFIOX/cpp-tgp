add_requires("boost")
add_rules("mode.debug")

set_toolchains("@llvm")


target("s5_1") do
    set_kind("binary")
    add_files("./s5_1.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s5_2") do
    set_kind("binary")
    add_files("./s5_2.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s5_3") do
    set_kind("binary")
    add_files("./s5_3.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s5_4") do
    set_kind("binary")
    add_files("./s5_4.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end