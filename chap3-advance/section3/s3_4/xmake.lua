add_requires("boost")
add_rules("mode.debug")

set_toolchains("@llvm")

target("s3_4") do
    set_kind("binary")
    add_files("./s3_4.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s3_5") do
    set_kind("binary")
    add_files("./s3_5.cxx")
    add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end