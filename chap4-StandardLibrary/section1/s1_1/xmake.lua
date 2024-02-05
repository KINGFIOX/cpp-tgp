add_rules("mode.debug")
set_toolchains("@llvm")

add_requires("boost")

target("s1_1") do
    set_kind("binary")
    add_files("./s1_1.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s1_2") do
    set_kind("binary")
    add_files("./s1_2.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s1_3") do
    set_kind("binary")
    add_files("./s1_3.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s1_4") do
    set_kind("binary")
    add_files("./s1_4.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s1_5") do
    set_kind("binary")
    add_files("./s1_5.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s1_6") do
    set_kind("binary")
    add_files("./s1_6.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end