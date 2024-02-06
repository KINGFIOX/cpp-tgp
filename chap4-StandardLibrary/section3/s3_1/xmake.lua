add_rules("mode.debug")
set_toolchains("@llvm")

add_requires("boost")

target("s3_1") do
    set_kind("binary")
    add_files("./s3_1.cxx")
    -- add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s3_2") do
    set_kind("binary")
    add_files("./s3_2.cxx")
    -- add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s3_3") do
    set_kind("binary")
    add_files("./s3_3.cxx")
    -- add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end

target("s3_4") do
    set_kind("binary")
    add_files("./s3_4.cxx")
    -- add_packages("boost")
	set_languages("cxx17")
	set_targetdir("./build")
end