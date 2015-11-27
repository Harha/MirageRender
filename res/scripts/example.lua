-- example.lua

function init()
	print("MirageRender example.lua script file initializing...")
	
	-- Create some test objects
	color_white = NewVector3(0.9, 0.9, 0.9)
	color_lamp1 = NewVector3(0.9, 0.75, 0.5)
	mat_diffuse_white = NewDiffMaterial(color_white)
	mat_emissiv_lamp1 = NewEmisMaterial(color_lamp1)
	mat_diffuse_red = NewDiffMaterial(NewVector3(0.9, 0.1, 0.1))
	
	q1 = NewQuaternion(0, 1, 0, 0)
	q2 = NewQuaternionLookAt(NewVector3(0, 0, 0), NewVector3(0, 1, 5))
	v4 = NewVector4(1, 0, 0, 1)
	tr = NewTransform(NewVector3(0, 0, 0), q1, NewVector3(1, 1, 1))
	
	camera = NewCameraPersp(tr, 1, 1, 70.0)
	
	print(camera)
end

function test()
	print("Hello world, from ",_VERSION,"! This is the test() function.\n")
end

function loop()
	for i = 0, 10, 1 do
		print("For loop: " .. i)
	end
end