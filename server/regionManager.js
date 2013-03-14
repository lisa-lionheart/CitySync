

var DATA_DIR = "../server_data/"


var fs = require("fs");
var AdmZip = require('adm-zip');
var child_proccess = require('child_process');

if(!fs.existsSync(DATA_DIR))
	fs.mkdir(DATA_DIR)


if(!fs.existsSync(DATA_DIR + "temp/"))
	fs.mkdir(DATA_DIR + "temp/" )


function regionDir(name) {
	return DATA_DIR + name + "/";
}

function tempDir()
{
	return DATA_DIR + "temp/";
}

function processCityIntoRegion(region, path, cb)
{
	//TODO: Do this with an NPM module
	var args = [ "--thumbnail", path, path];
	var scraperProc = child_proccess.spawn("./Sc4Scraper", args);

	scraperProc.on("exit",function(code){

		if(code != 0)
			return cb(false, "Scraper return an error")

		var meta = JSON.parse(scraperProc.stdout.read());

		console.log("City guid: " + meta.guid)

		var dataDir = regionDir(region) + meta.guid + "/";

		fs.mkdirSync( dataDir );

		fs.renameSync( path, dataDir + "0" );
		fs.renameSync( path + ".png", dataDir + "thumbnail.png");

		//TODO: Insert into a database

		cb(true);
	});

}

function createRegionFromZip(name, zip, cb)
{
	var zipEntries = zip.getEntries(); // an array of ZipEntry records

	var cityCount = 0;
	var cityProcessed = 0;

	if(fs.existsSync(regionDir(name)))
		return cb(false, "Region already exists");

	fs.mkdirSync(regionDir(name));


	function proccessed()
	{
		cityProcessed++;

		if(cityProcessed == cityCount)
			cb(true);
	}

	zipEntries.forEach(function(zipEntry) {

		if(zipEntry.name == "region.ini")
		{
		}
		else if(zipEntry.name.indexOf(".sc4") != -1)
		{
			cityCount++;

			var tempFilename = tempDir() + zipEntry.name;

			console.log("Extracted to " + tempFilename);
			zip.extractEntryTo(zipEntry.entryName, tempDir(),false,true);
			processCityIntoRegion(name,tempFilename,proccessed);
		}

	    console.log(zipEntry.toString()); // outputs zip entries information
	    
	});
}


function createRegion(name, request, response)
{
	console.log("Creating city named " + name);

	var zipName = DATA_DIR + "temp/" + name + "_temp.zip"
	var file = fs.createWriteStream(zipName)

	request.on("data",function(chunk){
		console.log("Recived " + chunk.length + " bytes of region " + name);
		file.write(chunk);
	});

	request.once("end",function(){

		file.end(function(){
			var zip = new AdmZip(zipName);
			createRegionFromZip(name, zip, function(success, message){

				fs.unlink(zipName);

  				response.writeHead( success ? 204 : 500 , message);
  				response.end();
			})
			
		});
	});

	return true;
}


exports.handleRequest = function(urlParts, request, response)
{

	if(urlParts[2]=="create")
		return createRegion(urlParts[3],request,response);

	return false;
}