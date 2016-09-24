-------------------------------------------------------------------------------
-- Cornellbox scene file for MirageRender
-- Author: harha
-------------------------------------------------------------------------------

function init()
	print("MirageRender cornellbox.lua script file init() function.")
	
	-- Define multithreading info
	SetMThreadInitInfo(16)
	
	-- Define display
	SetDisplayInitInfo(512/2, 512/2, 2)
	
	-- Define scene rendering settings
	SetRadianceClamping(10.0)
	SetMaxRecursion(10)
	
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
	v_camera = NewVector3(0, 1.5, 3.5)
	--q_camera = NewQuaternionLookAt(v_camera, NewVector3(0, 1.5, 0))
	q_camera = NewQuaternion(-0.00, -0.00, 1.00, -0.08)
	t_camera = NewTransform(v_camera, q_camera, v_full)
	c_perspective = NewCameraPersp(t_camera, 4, 64, 70.0)
	
	-- Meshes
	q_cornellbox = NewQuaternionLookAt(v_zero, NewVector3(0, 0, 1))
	t_cornellbox = NewTransform(v_zero, q_cornellbox, v_full)
	m_cornellbox = NewMesh(t_cornellbox, mat_diff_white, "cornellboxes/cornellbox_empty.obj")

	q_dragon = NewQuaternionLookAt(v_zero, NewVector3(-0.5, 0, -0.5))
	t_dragon = NewTransform(NewVector3(0.0, 0.0, -0.25), q_dragon, NewVector3(0.1, 0.1, 0.1))
	m_dragon = NewMesh(t_dragon, mat_gold, "dragon.obj")

	m_glassball = NewSphere(t_zero, mat_glass, NewVector3(-0.5, 0.25, 0.25), 0.25)
	
	-- Light sources
	t_sun = NewTransform(v_zero, NewQuaternion(0.41, 0.11, 0.86, -0.27), v_full)
	l_sun = NewLightDir(t_sun, NewVector3(1.0 * 5.5, 1.0 * 4.25, 1.0 * 4.0))
	t_point = NewTransform(NewVector3(0.0, 2.24, 0.0), q_idnt, v_full)
	l_point = NewLightPoint(t_point, NewVector3(1.0 * 5.0, 1.0 * 5.0, 1.0 * 5.0), 0, 0, 0.5)
	
	-- Add objects to scene
	AddMesh(m_cornellbox)
	AddMesh(m_dragon)
	AddShape(m_glassball)
	AddLight(l_sun)
	--AddLight(l_point)
	AddCamera(c_perspective)
	
	-- Build ray acceleration structure
	AddRayAccelerator("bvh", 1)
	
end