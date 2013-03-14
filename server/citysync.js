

var PORT = 8888;



var http = require("http")
var regionManager = require("./regionManager")


function routeRequest(request, response)
{
	var urlParts = request.url.split("/");

	console.log(urlParts);

	if(urlParts[1] == "region")
		return regionManager.handleRequest(urlParts,request,response);

	return false;
}


function onHttpRequest(request, response) {

  console.log(request.url);

  if(!routeRequest(request,response))
  {
  	  response.writeHead(404, {"Content-Type": "text/plain"});
	  response.write("Nothing to see here, move along citizen");
	  response.end();
  }
}



http.createServer(onHttpRequest).listen(PORT);