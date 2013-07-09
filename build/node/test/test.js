var util = require('util');
var nodeunit = require('nodeunit');
var geocached = require('../build/Release/geocached');


module.exports = exports = {
	setUp: function(callback) {
		callback();
	},
	tearDown: function(callback) {
		callback();
	}
};


exports.testDefineInterface = function(test) {
	test.expect(0);
	
	try
	{
	    geocached.dummy("hello");
	    
	}catch(e)
	{
	    console.log("EXCEPTION: " + util.inspect(e));
	}

	test.done();
}
