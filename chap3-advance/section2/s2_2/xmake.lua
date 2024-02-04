add_requires("boost")

target("s2_2") do
    set_kind("binary")
    add_files("./*.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end