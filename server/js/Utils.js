var fs = require("fs");
var exec = require('child_process').exec;

/**
	Load a json file
*/
exports.unserialiseJson = function(filename,def)
{
	var result = def;

	if(def == undefined)
		result = {}


	try {
		result = JSON.parse(fs.readFileSync(filename));
	}
	catch(e)
	{
		console.log("Utils.unserialiseJson: Could not open file " +  filename);
	}

	return result;
};

exports.saveJson = function(data,path)
{
	fs.writeFile(path, JSON.stringify(data));
};

exports.md5File = function(path,cb)
{
	exec("md5sum \"" + path + "\"",
			function (error, stdout, stderr) {

				if (error !== null) {
					console.log("md5 return an error");
					console.log(stdout);
					console.log(stderr);
					cb(false);
				}

				var md5 = stdout.split(" ")[0];

				console.log("File md5 for : " + path + " is " + md5);
				cb(md5);
			}
		);
}