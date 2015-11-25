-- test.lua

function init()
	io.write("Hello world, from ",_VERSION,"!\n")
end

function test()
	io.write("Hello world, from ",_VERSION,"! This is the test() function.\n")
end

function loop()
	for i = 0, 10, 1 do
		print("For loop: " .. i)
	end
end