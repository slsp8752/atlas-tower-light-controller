var express = require('express');
var req = require('request');
var app = express();

var url = 'https://api.particle.io/v1/devices/' + process.env.DEVICE_ID + "/recFrames?access_token=" + process.env.ACCESS_TOKEN;

app.set('port', (process.env.PORT || 5000));

app.use(express.static(__dirname + '/public'));

// views is directory for all template files
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

app.get('/', function(request, response) {
  response.render('pages/index');
});

app.get('/keyStatus', function(request, response) {
	var framepair = request.query.frame;
	req.post(url, { form:{arg:framepair} }, function(error, postResponse, body){ response.send(body); });	
});

//response.send(body);
//if(!error && response.statusCode == 200){

app.get('/send', function(request, response){
	response.render('pages/sending');
});


app.listen(app.get('port'), function() {
  console.log('Node app is running on port', app.get('port'));
});
