// Homeapp server

var fs = require('fs');
var sys = require('sys');
var exec = require('child_process').exec;

var express = require('express');
var app = express();
var bodyParser = require('body-parser');

var Datastore = require('nedb')
  , db = new Datastore({ filename: './homeapp.db', autoload: true });


app.use(bodyParser.json());
app.set('view engine', 'ejs');

app.get('/', function(req, res) {
  res.redirect('/signal/last')
});

app.get('/signal/last', function(req, res) {
  db.find({}).sort({ timestamp: -1 }).limit(1).exec(function (err, docs) {
    console.log(docs);
    res.render('index.html.ejs', docs[0]);
  });
});


app.post('/signal', function(req, res){
  console.log('Creating signal:');
  console.log(req.body);
  db.insert(req.body);
  res.status(200).end();
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

app.listen(3080);
console.log('Listening on port 3080');

