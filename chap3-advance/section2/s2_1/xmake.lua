add_requires("boost")

target("s2_1") do
    set_kind("binary")
    add_files("./s2_1.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end