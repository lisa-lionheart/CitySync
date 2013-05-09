
var Utils = require("./Utils");

function AuthManager(config)
{
	this.activeSesssion = {};

	this.users = Utils.unserialiseJson(config.data_dir + "users", {"admin":{"isAdmin":true, }})


	this.doLogin = function(request,response)
	{

	}

	this.getSession = function(request)
	{
		return {
			isAdmin: true
		};
	}


	this.onHttpRequest = function(request,response)
	{


		if(method == "login")
			return doLogin();

		return false;
	}
}

exports.AuthManager = AuthManager;