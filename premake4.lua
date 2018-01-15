-- Only works for linux - Windows is not yet supported.
solution "gKit2light"
	configurations { "debug", "release" }

	platforms { "x64", "x32" }
	
	includedirs { ".", "./Outerrain/Include", "./Dependencies/include", "/usr/include/"}
	
	rootDir = path.getabsolute("./Outerrain")
	
	configuration "debug"
		targetdir "bin/debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "release"
		targetdir "bin/release"
		flags { "OptimizeSpeed" }

	configuration "linux"
		buildoptions { "-mtune=native -march=native" }
		buildoptions { "-std=c++11" }
		buildoptions { "-W -Wall -Wsign-compare -Wno-unused-parameter -Wno-unused-variable" }
		buildoptions { "-flto"}
		linkoptions { "-flto"}
		links { "GLEW", "SDL2", "SDL2_image", "GL" }

	configuration { "linux", "debug" }
		buildoptions { "-g"}
		linkoptions { "-g"}
		
	configuration { "linux", "release" }
		buildoptions { "-fopenmp" }
		linkoptions { "-fopenmp" }

outerrainFiles = { rootDir .. "/Source/*.cpp", rootDir .. "/Include/*.h" }
project("Outerrain")
	language "C++"
	kind "ConsoleApp"
	targetdir "bin"
	files ( outerrainFiles )