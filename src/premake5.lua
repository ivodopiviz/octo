#!lua

workspace "Octo"
   configurations { "Debug", "Release" }

   project "Octo"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "**.cpp" }
	  includedirs { "depend/SDL2-2.0.5/include" }
	  libdirs { "depend/SDL2-2.0.5/lib/x86" }
      debugargs { "pong2.c8" }
      debugdir "../"
	  targetdir ("../")

      filter "configurations:Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
   		 targetsuffix "_d"
		 links { "SDL2", "SDL2main" }

      filter "configurations:Release"
         defines { "NDEBUG" }
         optimize "On"
		 links { "SDL2", "SDL2main" }