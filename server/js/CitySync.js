
var config = require("../config.json");

console.log(config);

var fs = require("fs");
var http = require("http")

var RegionManager = require("./RegionManager").RegionManager;
var AuthManager = require("./AuthManager").AuthManager;

function GameServer()
{
	if(!fs.existsSync(config.data_dir))
		fs.mkdir(config.data_dir)

	var authManger = new AuthManager(config);


	this.handlers = {
		region: new RegionManager(config),
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