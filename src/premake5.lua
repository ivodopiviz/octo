#!lua

workspace "Octo"
   configurations { "Debug", "Release" }

   project "Octo"
      kind "ConsoleApp"
      language "C++"
      files { "**.h", "**.cpp" }
	  includedirs { "depend/SDL2-2.0.5/include", "depend/SDL2_mixer-2.0.1/include", "depend/nativefiledialog-master/src/include" }
	  libdirs { "depend/SDL2-2.0.5/lib/x86", "depend/SDL2_mixer-2.0.1/lib/x86", "depend/nativefiledialog-master/build/lib/Release/x86" }
      debugargs { "pong2.c8" }
      debugdir "../"
	  targetdir ("../")

      filter "configurations:Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
   		 targetsuffix "_d"
		 links { "SDL2", "SDL2main", "SDL2_mixer", "nfd" }

      filter "configurations:Release"
         defines { "NDEBUG" }
         optimize "On"
		 links { "SDL2", "SDL2main", "SDL2_mixer", "nfd" }