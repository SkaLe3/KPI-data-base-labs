-- VMexternal.lua

-- Include directories relative to root folder (solution directory)
IncludeDir = {}

IncludeDir["Glad"] = "../vendor/Glad/include"
IncludeDir["glm"] = "../vendor/glm"
--IncludeDir["yaml_cpp"] = "Engine/vendor/yaml-cpp/include"



group "Dependencies"
	include "vendor/imgui"
	include "vendor/GLFW"
	include "vendor/Glad"
--	include "Engine/vendor/yaml-cpp"
group ""

group "Core"
include "VieM"

group ""
