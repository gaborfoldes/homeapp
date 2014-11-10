
var Firebase = require('firebase');
var db = new Firebase('https://vivid-fire-6554.firebaseio.com/');
db.child('devices').remove();
