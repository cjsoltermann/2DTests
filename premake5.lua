workspace "2DTests"
	configurations { "Debug", "Release" }

project "Squares"
	kind "WindowedApp"
	language "C++"
	files {
		"src/**.cpp",
		"src/**.c",
		"src/*.c",
	}
	libdirs "lib"
	links { 
		"dl",
		"GL",
		"glfw",
		"Box2D",
	}
	includedirs {
		"include",
		"Box2D"
	}
