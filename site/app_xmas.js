// Homeapp server

var express = require('express');
var app = express();
var bodyParser = require('body-parser');

var port = process.argv[2] || 3080;

app.use(bodyParser.json());
app.set('view engine', 'ejs');

app.get('/', function(req, res) {
  res.status(200).end();
});

app.get('/xmas', function(req, res) {
  res.render('xmas2015.html.ejs');
});

app.get('/xmas/on', function(req, res) {
    res.status(200).end();
});

app.get('/xmas/off', function(req, res) {
    res.status(200).end();
});

app.listen(port);
console.log('Listening on port ' + port);

