
var config = {
	port: 8888,
	data_dir: "/home/lisa/server_data/"
};

var fs = require("fs");
var http = require("http")
var sqlite3 = require("sqlite3").verbose();

var RegionManager = require("./RegionManager").RegionManager;
var AuthManager = require("./AuthManager").AuthManager;

function GameServer()
{
	if(!fs.existsSync(config.data_dir))
		fs.mkdir(config.data_dir)

	var database = new sqlite3.Database(config.data_dir + "database.db");

	var authManger = new AuthManager(config,database);


	this.handlers = {
		region: new RegionManager(config,database),
		auth: authManger
	};


	this.routeRequest = function(request, response)
	{
		request.urlParts = request.url.substr(1).split("/");

		var handler = this.handlers[request.urlParts[0]];
		if(handler)
			return handler.onHttpRequest(request,response);

		return false;
	}


	this.onHttpRequest = function(request, response) {

	  console.log(request.url);

	  request.session = authManger.getSession(request);

	  if(!this.routeRequest(request,response))
	  {
	  	  response.writeHead(404, {"Content-Type": "text/plain"});
		  response.write("Nothing to see here, move along citizen");
		  response.end();
	  }
	}

	this.run = function()
	{
		console.log("Running city sync server on port " + config.port);
		http.createServer(this.onHttpRequest.bind(this)).listen(config.port);
	}

	return this;
}

new GameServer().run();