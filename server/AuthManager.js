


function AuthManager(config, database)
{
	this.activeSesssion = {}	

	database.run("CREATE TABLE IF NOT EXISTS users (email TEXT, password TEXT, admin BOOLEAN)");


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