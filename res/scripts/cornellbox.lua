-------------------------------------------------------------------------------
-- Example scene file for MirageRender
-- Author: harha
-------------------------------------------------------------------------------

function init()
	print("MirageRender example.lua script file init() function.")
	
	-- Define multithreading info (0 for SDL_GetCPUCount())
	SetMThreadInitInfo(0)
	
	-- Define display
	SetDisplayInitInfo(512, 512, 1)
	
	-- Define scene rendering settings
	SetRadianceClamping(100.0)
	SetMaxRecursion(25)
	
	-- Some stuff with default values
	v_zero = NewVector3(0, 0, 0)
	v_full = NewVector3(1, 1, 1)
	q_idnt = NewQuaternion(1, 0, 0, 0)
	t_zero = NewTransform(v_zero, q_idnt, v_full)
	
	-- Colors
	col_max = NewVector3(1, 1, 1)
	col_white = NewVector3(0.9, 0.9, 0.9)
	
	-- Materials
	mat_diff_white = NewDiffMaterial(col_white)
	
	-- Cameras
	v_camera = NewVector3(0, 1.0, 4.0)
	q_camera = NewQuaternionLookAt(v_camera, NewVector3(0, 1.0, 0))
	t_camera = NewTransform(v_camera, q_camera, v_full)
	c_perspective = NewCameraPersp(t_camera, 4, 64, 70.0)
	
	-- Meshes
	q_cornellbox = NewQuaternionLookAt(v_zero, NewVector3(0, 0, 1))
	t_cornellbox = NewTransform(v_zero, q_cornellbox, v_full)
	m_cornellbox = NewMesh(t_cornellbox, mat_diff_white, "cornellboxes/cornellbox_original.obj")
	
	-- Add objects to scene
	AddMesh(m_cornellbox)
	AddCamera(c_perspective)
	
	-- Build ray acceleration structure
	AddRayAccelerator("bvh", 1)
	
end