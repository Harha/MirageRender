-------------------------------------------------------------------------------
-- Mitsuba scene file for MirageRender
-- Author: harha
-------------------------------------------------------------------------------

function init()
	print("MirageRender mitsuba.lua script file init() function.")
	
	-- Define multithreading info
	SetMThreadInitInfo(16)
	
	-- Define display
	SetDisplayInitInfo(512, 512, 1)
	
	-- Define scene rendering settings
	SetRadianceClamping(100.0)
	SetMaxRecursion(3)
	SetSceneSkyColor(NewVector3(0.1, 0.1, 0.1))
	
	-- Some stuff with default values
	v_zero = NewVector3(0, 0, 0)
	v_full = NewVector3(1, 1, 1)
	q_idnt = NewQuaternion(1, 0, 0, 0)
	t_zero = NewTransform(v_zero, q_idnt, v_full)
	
	-- Colors
	col_max = NewVector3(1, 1, 1)
	col_white = NewVector3(0.9, 0.9, 0.9)
	col_lime = NewVector3(0.25, 0.9, 0.175)
	col_gold = NewVector3(0.788, 0.537, 0.062)
	
	-- Materials
	mat_diff_white = NewDiffMaterial(col_white)
	mat_diff_lime = NewDiffMaterial(col_lime)
	mat_mirror = NewSpecMaterial(col_max)
	mat_glass = NewGlassMaterial(col_lime, 1.523)
	mat_gold = NewGlossyMaterial(col_gold, 0.075, 0.95, 0.005)
	
	-- Cameras
	v_camera = NewVector3(0, 2.25, 7.5)
	--q_camera = NewQuaternionLookAt(v_camera, NewVector3(0, 1.5, 0))
	q_camera = NewQuaternion(-0.00, -0.00, 1.00, -0.08)
	t_camera = NewTransform(v_camera, q_camera, v_full)
	c_perspective = NewCameraPersp(t_camera, 4, 64, 70.0)
	
	-- Meshes
	q_mitsuba = NewQuaternionLookAt(v_zero, NewVector3(0, 0, 1))
	t_mitsuba = NewTransform(v_zero, q_mitsuba, v_full)
	m_mitsuba = NewMesh(t_mitsuba, mat_diff_white, "mitsuba/mitsuba.obj")
	
	-- Light sources
	t_spot = NewTransform(NewVector3(-2.99, 3.68, 3.37), NewQuaternion(0.35, 0.12, 0.89, -0.26), v_full)
	l_spot = NewLightSpot(t_spot, NewVector3(5.0, 5.0, 5.0), 0, 0, 0.1, 0.8);
	
	-- Add objects to scene
	AddMesh(m_mitsuba)
	AddLight(l_spot)
	AddCamera(c_perspective)
	
	-- Build ray acceleration structure
	AddRayAccelerator("bvh", 1)
	
end