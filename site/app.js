var fs = require('fs');
var express = require('express');
var app = express();
var sys = require('sys');
var exec = require('child_process').exec;

app.engine('hamlc', require('haml-coffee').__express);

app.get('/', function(req, res) {
//  res.send(200);
  fs.readFile('/data/test_temp', 'utf-8', function(err, data) {
    if (err) throw err;
    var lines = data.trim().split('\n');
    var lastLine = lines.slice(-1);
    res.render('index.hamlc', JSON.parse(lastLine));
  });
});

app.get('/xmas/:state', function(req, res){
  var onoff = (req.params.state === "on") ? "on" : "off";
  var child = exec("/home/pi/code/homeapp/controller-pi/light " + onoff, function (error, stdout, stderr) {
    console.log('stdout: ' + stdout);
    console.log('stderr: ' + stderr);
    if (error !== null) {
      console.log('exec error: ' + error);
    }
    res.send(200);
  })
});

app.listen(80);
console.log('Listening on port 80');

