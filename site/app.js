// Homeapp server

var fs = require('fs');
var sys = require('sys');
var exec = require('child_process').exec;

var express = require('express');
var app = express();
var bodyParser = require('body-parser');

var Datastore = require('nedb')
  , db = new Datastore({ filename: '/data/homeapp.db', autoload: true });


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

app.listen(80);
console.log('Listening on port 80');

