
var fs = require("fs");
var AdmZip = require('adm-zip');
var child_proccess = require('child_process');
var Utils = require("./Utils");


function RegionManager(config)
{
	if(!fs.existsSync(config.data_dir + "temp/"))
		fs.mkdir(config.data_dir + "temp/" )

	var regions = Utils.unserialiseJson(config.data_dir + "regions");

	function saveRegionFile()
	{
		Utils.saveJson(regions, config.data_dir + "regions")
	}

	function regionDir(name) {
		return config.data_dir + name + "/";
	}

	function tempDir()
	{
		return config.data_dir + "temp/";
	}

    function processCityIntoRegion(region, path, cb)
	{
		var cmd = 'Sc4Scraper --thumbnail "' + path + '" "' + path + '"';

		console.log("Scraper command:" + cmd);

		child = child_proccess.exec(cmd,
		  function (error, stdout, stderr) {
		    if (error !== null) {
					console.log("Scraper return an error");
				console.log(stdout);
				return cb(false, "Scraper returned an error");
			}

			var cityJson = JSON.parse(stdout);

			cityJson.revision = 0;

			console.log("City guid: " + cityJson.guid)
			console.log(cityJson);

			var dataDir = regionDir(region.name) + cityJson.guid + "/";

			fs.mkdirSync( dataDir );

			//Initial city revison
			fs.renameSync( path, dataDir + "0" );

			//The thumbnail
			fs.renameSync( path + ".png", dataDir + "thumbnail.png");

			//Finally, md5 the city
			Utils.md5File(dataDir + "0", function(md5){

				cityJson.md5sum = md5;
				cb(cityJson);
			})			

		});
	}


	function extractRegionFromZip(name, zip, cb)
	{
		var zipEntries = zip.getEntries(); // an array of ZipEntry records

		var cityCount = 0;
		var cityProcessed = 0;

		if(fs.existsSync(regionDir(name)))
			return cb(false, "Region already exists");

		var region = {
			name: name,
			cities: {}
		};

		fs.mkdirSync(regionDir(name));

		function proccessed(cityJson)
		{
			if(region)
				region.cities[cityJson.guid] = cityJson;

			cityProcessed++;

			if(cityProcessed == cityCount){
				cb(region);
			}
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
				processCityIntoRegion(region,tempFilename,proccessed);
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
				extractRegionFromZip(name, zip, function(region, message){


					fs.unlink(zipName);

					if(region)
					{
						regions[name] = region;
						saveRegionFile();
						
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

			response.writeHead( 200, {
				"Content-Length": data.length
			});
		  	response.write(data);
			response.end();
		});

		return true;
	}

	function getCity(regionName,guid,request,response)
	{

		console.log("Get city",guid);


		var city = regions[regionName].cities[guid];

		if(!city){
			response.writeHead(404,"City not found");
			response.end();
			return;
		}

		console.log("Get city",city);

		var path = regionDir(regionName) + "/" + guid + "/" + city.revision;
		return getFile(path,response);
	}

	function getCityFile(region,guid, name, request,response)
	{
		var path = regionDir(region) + "/" + guid + "/" + name;
		getFile(path,response);
		return true;
	}

	function uploadCity(regionName,guid,request,response)
	{
		var tempName = tempDir() + guid + "_temp";
		var file = fs.createWriteStream(zipName)

		var city = regions[regionName][guid];

		if(!city){
			response.writeHead(404,"City not found");
			response.end();
			return;
		}

		request.on("data",function(chunk){
			console.log("Recived " + chunk.length + " bytes of city " + guid);
			file.write(chunk);
		});

		request.once("end",function(){
			file = null;

			Utils.md5File(tempName,function(md5){

				//TODO: versioning, hash shceking yada yada
				city.revision++;
				city.md5sum = md5;

				saveRegionFile();

				var path = regionDir(region) + "/" + guid + "/" + city.revision;
				fs.renameSync(tempName, path);

				response.writeHead( 200 );	
				response.end();
			});


		});
	}

	function getRegion(name,request,response)
	{
		if(regions[name])
		{
			response.writeHead( 200 );
			response.write( JSON.stringify(regions[name]) );
		}
		else
		{
			console.log(err);
			response.writeHead( 500 , err);
		}

		response.end();
	
		return true;
	}


	function getRegionList(request,response)
	{
		response.writeHead( 200 );
		response.write( JSON.stringify(regions) );
		response.end();
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
		{

			if(request.method == "POST")
			{
				return uploadCity(region, guid, request, response);
			}

			if(request.method == "GET")
			{
				if(guid == "config.bmp")
					return getFile(regionDir(region) + "/config.bmp", response);

				if(guid == "desc.html")
					return getFile(regionDir(region) + "/desc.html", response);

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


