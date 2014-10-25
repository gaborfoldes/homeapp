// Homeapp server

var express = require('express');
var app = express();
var bodyParser = require('body-parser');

var Datastore = require('nedb')
  , db = new Datastore({ filename: '/data/homeapp.db', autoload: true });

var port = process.argv[2] || 3080;

app.use(bodyParser.json());
app.set('view engine', 'ejs');

app.get('/', function(req, res) {
  //res.redirect('/dashboard')
  res.status(200).end();
});

app.get('/devices/:id', function(req, res) {
  db.find({ "device.id": "f0f0f0f0e" + req.params.id }).sort({ timestamp: -1 }).limit(1440).exec(function (err, data) {
    res.render('device.html.ejs', { signals: data });
  });
});


app.listen(port);
console.log('Listening on port ' + port);

