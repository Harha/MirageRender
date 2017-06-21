-------------------------------------------------------------------------------
-- Sponza scene file for MirageRender
-- Author: harha
-------------------------------------------------------------------------------

function init()
	print("MirageRender sponza.lua script file init() function.")
	
	-- Define multithreading info (0 for SDL_GetCPUCount())
	SetMThreadInitInfo(0)
	
	-- Define display
	SetDisplayInitInfo(256, 256, 2)
	
	-- Define scene rendering settings
	SetRadianceClamping(5.0)
	SetMaxRecursion(5)
	SetSceneSkyColor(NewVector3(12.0, 11.0, 10.0))
	
	-- Some stuff with default values
	v_zero = NewVector3(0, 0, 0)
	v_full = NewVector3(1, 1, 1)
	q_idnt = NewQuaternion(1, 0, 0, 0)
	t_zero = NewTransform(v_zero, q_idnt, v_full)
	
	-- Colors
	col_max = NewVector3(1, 1, 1)
	col_white = NewVector3(0.9, 0.9, 0.9)
	col_green = NewVector3(0.25, 0.9, 0.175)
	
	-- Materials
	mat_diff_white = NewDiffMaterial(col_white)
	mat_diff_green = NewDiffMaterial(col_green)
	mat_mirror = NewSpecMaterial(col_max)
	mat_glass = NewDielectricMaterial(col_max, 1.52)
	
	-- Cameras
	v_camera = NewVector3(12.56, 1.66, -1.77)
	q_camera = NewQuaternion(0.79, -0.03, -0.62, -0.02)
	t_camera = NewTransform(v_camera, q_camera, v_full)
	c_perspective = NewCameraPersp(t_camera, 4, 64, 70.0)
	
	-- Meshes
	q_sponza = NewQuaternionLookAt(v_zero, NewVector3(0, 0, 1))
	t_sponza = NewTransform(v_zero, q_sponza, v_full)
	m_sponza = NewMesh(t_sponza, mat_diff_white, "sponza_dabrovic/dabrovic_sponza.obj")

	q_dragon = NewQuaternionLookAt(v_zero, NewVector3(-0.5, 0, -0.5))
	t_dragon = NewTransform(NewVector3(0.5, 0.0, 0.25), q_dragon, NewVector3(0.25, 0.25, 0.25))
	m_dragon = NewMesh(t_dragon, mat_diff_green, "dragon.obj")
	
	-- Light sources
	t_sun = NewTransform(v_zero, NewQuaternion(0.68, 0.46, -0.47, 0.31), v_full)
	l_sun = NewLightDir(t_sun, NewVector3(12.0, 11.0, 10.0))
	
	-- Add objects to scene
	AddMesh(m_sponza)
	AddMesh(m_dragon)
	--AddLight(l_sun)
	AddCamera(c_perspective)
	
	-- Build ray acceleration structure
	AddRayAccelerator("bvh", 1)
	
end