-- example.lua

function init()
	print("MirageRender example.lua script file init() function.")
	
	-- Define multithreading info
	SetMThreadInitInfo(16) -- Render thread amount
	
	-- Define display
	SetDisplayInitInfo(512, 512, 1) -- resX, resY, scale
	
	-- Define scene settings
	SetRadianceClamping(1.0) -- Floating point value, lesser = unrealistic, but most noise goes nearly instantly away.
							 -- !.0 Is not that realistic, especially with indirect lighting.
	SetMaxRecursion(10) -- Integer value, maximum recursion per ray, usually 5-10 is fine.
	
	-- Initialize objects
	v_zero = NewVector3(0, 0, 0)
	v_full = NewVector3(1, 1, 1)
	q_idnt = NewQuaternion(1, 0, 0, 0)
	t_zero = NewTransform(v_zero, q_idnt, v_full)
	
	col_max = NewVector3(1, 1, 1)
	col_white = NewVector3(0.9, 0.9, 0.9)
	col_red = NewVector3(0.9, 0.1, 0.1)
	col_green = NewVector3(0.1, 0.9, 0.1)
	col_blue = NewVector3(0.1, 0.1, 0.9)
	col_silver = NewVector3(0.75, 0.75, 0.75);
	col_gold = NewVector3(0.9, 0.70, 0.25);
	
	mat_diff_white = NewDiffMaterial(col_white)
	mat_diff_red = NewDiffMaterial(col_red)
	mat_diff_green = NewDiffMaterial(col_green)
	mat_diff_blue = NewDiffMaterial(col_blue)
	mat_glossy_silver = NewGlossyMaterial(col_silver, 0.25, 0.9, 0.1)
	
	mat_gold_len = 8
	mat_gold_array = {}
	for i = 1, mat_gold_len do
		color = col_gold
		if i == 1 then
			color = col_silver
		end
		table.insert(mat_gold_array, NewGlossyMaterial(color, 0.05 * i, 0.9, 0.1))
	end
	
	mat_mirror = NewSpecMaterial(col_max)
	mat_glass = NewGlassMaterial(col_max, 1.52)
	
	-- Cameras
	v_camera = NewVector3(0, 1, 4)
	t_camera = NewTransform(v_camera, NewQuaternionLookAt(v_camera, NewVector3(0, 1, 0)), v_full)
	t_cameraO = NewTransform(v_camera, NewQuaternion(1, 0, 0, 0), v_full)
	camera_persp = NewCameraPersp(t_camera, 4, 64, 70.0)
	camera_ortho = NewCameraOrtho(t_cameraO, 4, 64, 0.01)
	
	-- Meshes
	q_cbox = NewQuaternionLookAt(v_zero, NewVector3(0, 0, 1))
	t_cbox = NewTransform(v_zero, q_cbox, v_full)
	mesh_cbox = NewMesh(t_cbox, mat_diff_white, "cornellbox_empty.obj")
	
	v_dragon = NewVector3(-0.5, 0, 0.5)
	t_dragon = NewTransform(v_dragon, NewQuaternionLookAt(v_dragon, NewVector3(-1, 0, 1)), NewVector3(0.05, 0.05, 0.05))
	mesh_dragon = NewMesh(t_dragon, mat_diff_white, "dragon.obj")
	
	for i = 1, mat_gold_len do
		ii = i - 1
		AddShape(NewSphere(t_zero, mat_gold_array[i], NewVector3(-0.75 + ii * 0.225, 0.5 + 0.25 * math.sin(i * 0.825), -0.25), 0.2))
	end
	
	-- Light sources
	t_plight1 = NewTransform(NewVector3(0.0, 2.2, 0.0), q_idnt, v_full)
	plight1 = NewLightPoint(t_plight1, NewVector3(20 * 2, 17.5 * 2, 16.25 * 2), 0, 0, 5.0)
	
	-- Add objects to scene
	AddMesh(mesh_cbox)
	--AddMesh(mesh_dragon)
	--AddShape(sphere1)
	AddLight(plight1)
	AddCamera(camera_persp)
	
	-- Build ray acceleration structure
	AddRayAccelerator("k-d_tree")
	
end
