-- example.lua

function init()
	print("MirageRender example.lua script file init() function.")
	
	-- Define scene settings
	setRadianceClamping(3.0) -- Floating point value, lesser = unrealistic, but most noise goes nearly instantly away.
	setMaxRecursion(25) -- Integer value, maximum recursion per ray, usually 5-10 is fine.
	
	-- Initialize objects
	v_zero = NewVector3(0, 0, 0)
	v_full = NewVector3(1, 1, 1)
	q_idnt = NewQuaternion(1, 0, 0, 0)
	t_zero = NewTransform(v_zero, q_idnt, v_full)
	
	col_white = NewVector3(0.9, 0.9, 0.9)
	col_red = NewVector3(0.9, 0.1, 0.1)
	col_green = NewVector3(0.1, 0.9, 0.1)
	col_blue = NewVector3(0.1, 0.1, 0.9)
	
	mat_diff_white = NewDiffMaterial(col_white)
	mat_diff_red = NewDiffMaterial(col_red)
	mat_diff_green = NewDiffMaterial(col_green)
	mat_diff_blue = NewDiffMaterial(col_blue)
	
	-- Cameras
	v_camera = NewVector3(0, 1, 4)
	t_camera = NewTransform(v_camera, NewQuaternionLookAt(v_camera, NewVector3(0, 1, 0)), v_full)
	camera_persp = NewCameraPersp(t_camera, 4, 64, 70.0)
	camera_ortho = NewCameraOrtho(t_camera, 4, 64, 0.01)
	
	-- Meshes
	q_cbox = NewQuaternionLookAt(v_zero, NewVector3(0, 0, 1))
	t_cbox = NewTransform(v_zero, q_cbox, v_full)
	mesh_cbox = NewMesh(t_cbox, mat_diff_white, "cornellbox_nolight.obj")
	
	v_dragon = NewVector3(-0.5, 0, 0.5)
	t_dragon = NewTransform(v_dragon, NewQuaternionLookAt(v_dragon, NewVector3(-1, 0, 1)), NewVector3(0.05, 0.05, 0.05))
	mesh_dragon = NewMesh(t_dragon, mat_diff_white, "dragon.obj")
	
	-- Light sources
	t_plight1 = NewTransform(NewVector3(0.0, 1.95, 0.0), q_idnt, v_full)
	plight1 = NewLightPoint(t_plight1, NewVector3(20, 20.0, 20.0), 0, 0, 10.0)
	
	-- Add objects to scene
	AddMesh(mesh_cbox)
	AddMesh(mesh_dragon)
	AddLight(plight1)
	AddCamera(camera_persp)
	
	-- Build ray acceleration structure
	AddRayAccelerator("k-d_tree")
	
end
