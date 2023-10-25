-- premake5.lua
workspace "MusiCatalog"
	architecture "x64"
	configurations { "Debug", "Release", "Dist" }
	startproject "MusCat"



outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "VMexternal.lua"
include "MusCat"








