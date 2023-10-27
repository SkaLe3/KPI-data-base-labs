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
		"src"
	}

	libdirs 
	{
		os.getenv("POSTGRESQL_LIB")
	}
	links
	{
		"VieM",
		"libpq"
		
	}

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	filter "system:windows"
		systemversion "latest"
        defines { "VM_PLATFORM_WINDOWS" }


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
