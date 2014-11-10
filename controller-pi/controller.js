
// Set up radio

var nrf = require('nrf');

var spiDev = "/dev/spidev0.0";
var cePin = 24;
var irqPin = 25;

var radio = nrf.connect(spiDev, cePin, irqPin);
radio.channel(0x4c);
radio.transmitPower('PA_HIGH');
radio.dataRate('1Mbps');
radio.crcBytes(2);
radio.autoRetransmit({
	count: 15,
	delay: 4000
});


// Set up data store

//var Datastore = require('nedb')
//  , db = new Datastore({ filename: '/data/homeapp.db', autoload: true });

var Firebase = require('firebase');
var db = new Firebase('https://vivid-fire-6554.firebaseio.com/');


// Handlers

function store_signal (d) {

	var device = this._addr.toString('hex'),
            c = d.readUInt32BE(0)/100,
            f = (c * 9 / 5) + 32;

	db.child('signals/' + device).push().setWithPriority(
	    { timestamp: new Date().toJSON(), data: {celsius: c, fahrenheit: f} },
	    Firebase.ServerValue.TIMESTAMP
	);
	console.log('Device #' + device + ' -- temperature: ' + c + ' C ' + f + ' F');
}

function error_handler (e) {
	console.log("Error:", e);
}
 
 
// Start the radio

radio.begin(function() {
 
	console.log('Starting radio...');

	var tx1 = radio.openPipe('tx', 0xF0F0F0F0D2);
	db.child('devices/' + tx1._addr.toString('hex') + '/name').set("Heater");
	db.child('devices/' + tx1._addr.toString('hex') + '/type').set("HVAC");

	var rx1 = radio.openPipe('rx', 0xF0F0F0F0E1);
	db.child('devices/' + rx1._addr.toString('hex') + '/name').set("Kiki's room");
	db.child('devices/' + rx1._addr.toString('hex') + '/type').set("Temperature");
	rx1.on('data', store_signal).on('error', error_handler);

	var rx3 = radio.openPipe('rx', 0xF0F0F0F0E3);
	db.child('devices/' + rx3._addr.toString('hex') + '/name').set("Living room");
	db.child('devices/' + rx3._addr.toString('hex') + '/type').set("Temperature");
	rx3.on('data', store_signal).on('error', error_handler);

	rx1.on('data', function (d) {
            var c = d.readUInt32BE(0)/100,
                f = (c * 9 / 5) + 32;

            if (f < 70.5) {
                var buf = new Buffer(4);
                buf.writeUInt32BE(1, 0);
                console.log("Turning heat on");
                tx1.write(buf);
		db.child('signals/' + tx1._addr.toString('hex')).push().setWithPriority(
		    { timestamp: new Date().toJSON(), sent: {heat: 1} },
		    Firebase.ServerValue.TIMESTAMP
		);
            } else if (f > 73.5) {
                var buf = new Buffer(4);
                buf.writeUInt32BE(0, 0);
                console.log("Turning heat off");
                tx1.write(buf);
		db.child('signals/' + tx1._addr.toString('hex')).push().setWithPriority(
		    { timestamp: new Date().toJSON(), sent: {heat: 0} },
		    Firebase.ServerValue.TIMESTAMP
		);
            }
	});

});



