var express = require('express');
var req = require('request');
var app = express();

app.set('port', (process.env.PORT || 5000));

app.use(express.static(__dirname + '/public'));

// views is directory for all template files
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

app.get('/', function(request, response) {
  response.render('pages/index');
});


//response.send(body);
//if(!error && response.statusCode == 200){

app.get('/send', function(request, response){
var url = 'https://api.particle.io/v1/devices/' + process.env.DEVICE_ID + "/functionName?access_token=" + process.env.ACCESS_TOKEN;

if(request.query.framepair0 != "") req.post(url, {form:{arg:request.query.framepair0} }, function(error, postResponse, body){});

if(request.query.framepair1 != "") req.post(url, {form:{arg:request.query.framepair1} }, function(error, postResponse, body){});
	  
if(request.query.framepair2 != "") req.post(url, {form:{arg:request.query.framepair2} }, function(error, postResponse, body){});
	  
if(request.query.framepair3 != "") req.post(url, {form:{arg:request.query.framepair3} }, function(error, postResponse, body){});
			  
if(request.query.framepair4 != "") req.post(url, {form:{arg:request.query.framepair4} }, function(error, postResponse, body){});

response.send("Sent");
});


app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});
