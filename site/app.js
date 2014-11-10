// Homeapp server

var express = require('express');
var app = express();
var bodyParser = require('body-parser');

// var Datastore = require('nedb')
//   , db = new Datastore({ filename: '/data/homeapp.db', autoload: true });

//var Firebase = require('firebase');
//var db = new Firebase('https://vivid-fire-6554.firebaseio.com/');

var port = process.argv[2] || 3080;

app.use(bodyParser.json());
app.set('view engine', 'ejs');

app.get('/', function(req, res) {
  //res.redirect('/dashboard')
  res.status(200).end();
});

app.get('/devices/:id', function(req, res) {

  res.render('device.html.ejs', { device: {name: 'Test', id: 'f0f0f0f0e' + req.params.id } });


//  db.child('devices/f0f0f0f0e' + req.params.id + '/signals').startAt(Date.now()-86400000).once('value', function (data) {
//  	console.log('GET /devices/' + req.params.id );
//    res.render('device.html.ejs', { device: {name: 'Test', id: 'f0f0f0f0e' + req.params.id } });
//  }, function (e) {
//    console.log('The read failed: ' + e.code);
//  });

  // db.find({ "device.id": "f0f0f0f0e" + req.params.id }).sort({ timestamp: -1 }).limit(1440).exec(function (err, data) {
  //   res.render('device.html.ejs', { signals: data });
  // });
});


app.listen(port);
console.log('Listening on port ' + port);

