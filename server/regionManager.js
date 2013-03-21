
var fs = require("fs");
var AdmZip = require('adm-zip');
var child_proccess = require('child_process');


var exec = require('child_process').exec;


function RegionManager(config, database)
{
	

	if(!fs.existsSync(config.data_dir + "temp/"))
		fs.mkdir(config.data_dir + "temp/" )


	database.run("CREATE TABLE IF NOT EXISTS regions (name TEXT)",console.log);
	database.run("CREATE TABLE IF NOT EXISTS cities (name TEXT, guid TEXT, region TEXT, owner TEXT, x INT, y INT, size INT)",console.log);


	function regionDir(name) {
		return config.data_dir + name + "/";
	}

	function tempDir()
	{
		return config.data_dir + "temp/";
	}

	function sqlCB(error)
	{
		console.log(error);
	}


    function processCityIntoRegion(regionName, path, cb)
	{
		//TODO: Do this with an NPM module
		//var args = [ "--thumbnail", path, path];
		//var scraperProc = child_proccess.spawn("./Sc4Scraper", args);

		var cmd = './Sc4Scraper --thumbnail "' + path + '" "' + path + '"';

		console.log("Scraper command:" + cmd);

		child = exec(cmd,
		  function (error, stdout, stderr) {
		    if (error !== null) {
					console.log("Scraper return an error");
				console.log(stdout);
				return cb(false, "Scraper returned an error");
			}

			var cityJson = JSON.parse(stdout);

			console.log("City guid: " + cityJson.guid)
			console.log(cityJson);

			var dataDir = regionDir(regionName) + cityJson.guid + "/";

			fs.mkdirSync( dataDir );

			fs.renameSync( path, dataDir + "0" );
			fs.renameSync( path + ".png", dataDir + "thumbnail.png");

			database.run(
				"INSERT INTO cities VALUES (?,?,?,?,?,?,?)",
			[
				cityJson.name,
				cityJson.guid,
				regionName,
				"",
				cityJson.tileX,
				cityJson.tileY, 
				cityJson.sizeX
			]);

			cb(cityJson);
		});

/*
		scraperProc.on("exit",function(code){


		});
	*/
	}


	function createRegionFromZip(name, zip, cb)
	{
		var zipEntries = zip.getEntries(); // an array of ZipEntry records

		var cityCount = 0;
		var cityProcessed = 0;

		var regionJson = {
			name: name,
			cities: {}
		};


		if(fs.existsSync(regionDir(name)))
			return cb(false, "Region already exists");

		fs.mkdirSync(regionDir(name));

		function proccessed(cityJson)
		{
			if(cityJson)
				regionJson.cities[cityJson.guid] = cityJson;

			cityProcessed++;

			if(cityProcessed == cityCount)
				cb(regionJson);
		}

		zipEntries.forEach(function(zipEntry) {

			if(zipEntry.name == "region.ini" || zipEntry.name == "config.bmp")
			{
				zip.extractEntryTo(zipEntry.entryName, regionDir(name),false,true);
			}
			else if(zipEntry.name.indexOf(".sc4") != -1)
			{
				cityCount++;

				var tempFilename = tempDir() + zipEntry.name;

				console.log("Extracted to " + tempFilename);
				zip.extractEntryTo(zipEntry.entryName, tempDir(),false,true);
				processCityIntoRegion(name,tempFilename,proccessed);
			}		    
		});
	}


	function createRegion(name, request, response)
	{
		console.log("Creating city named " + name);


		var zipName = tempDir() + name + "_temp.zip"
		var file = fs.createWriteStream(zipName)

		request.on("data",function(chunk){
			console.log("Recived " + chunk.length + " bytes of region " + name);
			file.write(chunk);
		});

		request.once("end",function(){

			file.end(function(){
				var zip = new AdmZip(zipName);
				createRegionFromZip(name, zip, function(region, message){

					fs.unlink(zipName);

					database.run(
						"INSERT INTO regions VALUES (?)",
						[
							name
						]
					);

					if(region)
					{
						response.writeHead( 200 );
						response.write( JSON.toString(region) );
					}
					else
					{
	  					response.writeHead( 500 , message);
					}

	  				response.end();
				})
				
			});
		});

		return true;
	}

	function getFile(path,response)
	{
		if(!fs.existsSync( path ))
		{
			response.writeHead( 404 , "File not present");
		}

		fs.readFile(path, function (err, data) {

			if(err)
			{
				console.log("Error reading file " + err)
				response.writeHead( 500 );
		  		response.end();
		  		return;
			}

			console.log("Sending data length:" + data.length);
			response.writeHead( 200 );
		  	response.write(data);
			response.end();
		});

		return true;
	}

	function getCity(region,guid,request,response)
	{
		var path = regionDir(region) + "/" + guid + "/0";
		return getFile(path,response);
	}

	function getCityFile(region,guid, name, request,response)
	{
		var path = regionDir(region) + "/" + guid + "/" + name;
		getFile(path,response);
		return true;
	}

	function uploadCity(region,guid,request,response)
	{
		var tempName = tempDir() + guid + "_temp";
		var file = fs.createWriteStream(zipName)

		request.on("data",function(chunk){
			console.log("Recived " + chunk.length + " bytes of city " + guid);
			file.write(chunk);
		});

		request.once("end",function(){
			file = null;

			//TODO: versioning, hash shceking yada yada
			var path = regionDir(region) + "/" + guid + "/0"
			fs.unlinkSync(path);
			fs.renameSync(tempName, path);
		});
	}

	function getRegion(name,request,response)
	{
		database.all("SELECT * FROM cities WHERE region='" + name + "'", function(err,rows){

			if(!err)
			{

				response.writeHead( 200 );
				response.write( JSON.stringify(rows) );
			}
			else
			{

				console.log(err);
				response.writeHead( 500 , err);
			}

			response.end();
		});

		return true;
	}


	function getRegionList(request,response)
	{
		database.all("SELECT * FROM regions", function(err,rows){

			if(!err)
			{
				response.writeHead( 200 );
				response.write( JSON.stringify(rows) );
			}
			else
			{

				console.log(err);
				response.writeHead( 500 , err);
			}

			response.end();
		});

		return true;
	}

	this.onHttpRequest = function(request, response)
	{
		var region = request.urlParts[1];
		var guid = request.urlParts[2];
		var argc = request.urlParts.length;

		if(argc == 1){
			return getRegionList(request,response);
		}

		if(argc == 2){

			if(request.method == "POST")
				return createRegion(request.urlParts[1],request,response);

			if(request.method == "GET")
				return getRegion(region,request,response);
		}

		if(argc == 3)
		{;

			if(request.method == "GET")
			{
				if(guid == "config.bmp")
					return getFile(regionDir(region) + "/config.bmp", response);

				return getCity(region,guid,request,response);
			}
		}

		if(argc == 4)
		{

			if(request.method == "GET")
				return getCityFile(region,guid,request.urlParts[3],request,response);
		}

		return false;
	}

	return this;
}

exports.RegionManager = RegionManager;


