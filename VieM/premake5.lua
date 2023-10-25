project "VieM"
	kind "StaticLib"
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
        "src",
        "../vendor/imgui",
        "../vendor/GLFW/include",
        "../vendor/stb_image",
        "%{IncludeDir.Glad}",
		"%{IncludeDir.glm}"
        --"%{IncludeDir.yaml_cpp}",
    }

	defines
	{
        "_CRT_SECURE_NO_WARNINGS"
	}
    

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		--"yaml-cpp",
		"opengl32.lib"
	}
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	filter "system:windows"
		systemversion "latest"

		defines { "GLFW_INCLUDE_NONE", "VM_PLATFORM_WINDOWS"}

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

