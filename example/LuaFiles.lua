intVal = 5 + 10
stringVal = "Andy " .. intVal
boolVal = true;

Array = { 'a','b','c','d'}

-- Table = { intVal = 8, stringVal = "Andy", boolVal = true }
Table = {}
Table["Name"] = "Andy"
Table["Int"] = 15+2

function add(x,y)
	return x + y;
end

function check(x)
	if x == true then return "trueString" else return "falseString" end
end
function string(x)
	return x;
end
