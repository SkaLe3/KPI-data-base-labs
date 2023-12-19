project "MusCat"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
	staticruntime "off"

	

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"../VieM/src",
		"../vendor/imgui",
		"../vendor/GLFW/include",
		"%{IncludeDir.glm}",
		"../vendor",
		"%{IncludeDir.PostgreSQL}",
        "%{IncludeDir.TinyORM}",
        os.getenv("Qt") .. "/include",
        os.getenv("Qt") .. "/include/QtCore",
        os.getenv("Qt") .. "/include/QtSql",
		"src"
	}

	libdirs 
	{
		os.getenv("POSTGRESQL_LIB")
	}
	links
	{
		"VieM",
		"libpq",
        "TinyORM"

		
	}

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	filter "system:windows"
		systemversion "latest"
        defines { "VM_PLATFORM_WINDOWS" }
        buildoptions {"/Zc:__cplusplus"}


	filter "configurations:Debug"
		defines "VM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "VM_RELEASE"
		runtime "Release"
		optimize "on"
        symbols "on"

    filter "configurations:Dist"
		defines "VM_Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
        
    filter { "toolset:msc*" }
        buildoptions { "/bigobj" }