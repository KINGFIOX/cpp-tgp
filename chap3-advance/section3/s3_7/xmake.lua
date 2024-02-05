-- add_requires("boost")
add_rules("mode.debug")

set_toolchains("@llvm")


target("s3_7") do
    set_kind("binary")
    add_files("./s3_7.cxx")
    -- add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s3_8") do
    set_kind("binary")
    add_files("./s3_8.cxx")
    -- add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end