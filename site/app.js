var fs = require('fs');
var express = require('express');
var app = express();

app.engine('hamlc', require('haml-coffee').__express);

app.get('/', function(req, res){
  fs.readFile('/data/test_temp', 'utf-8', function(err, data) {
    if (err) throw err;

    var lines = data.trim().split('\n');
    var lastLine = lines.slice(-1);
    res.render('index.hamlc', JSON.parse(lastLine));
  });
});

app.listen(8080);
console.log('Listening on port 80');

