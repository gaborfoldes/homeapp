
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

function process_signal (d) {
	device = this._addr.toString('hex');
	c = d.readUInt32BE(0)/100;
	signal = { timestamp: new Date().toJSON(),
//		   device: {
//		     id: this._addr.toString('hex'),
//		     name: this._name
//		   },
		   data: {
		     celsius: c,
		     fahrenheit: (c * 9 / 5 ) + 32
		   }
		 };
	db.child('devices/' + device + '/signals').push(signal);
}

function error_handler (e) {
	console.log("Error:", e);
}
 
 
// Start the radio

radio.begin(function() {
 
	console.log('Starting radio...');

	var rx1 = radio.openPipe('rx', 0xF0F0F0F0E1); // Listen at address 
	var rx3 = radio.openPipe('rx', 0xF0F0F0F0E3); // Listen at address 

	db.child('devices/' + rx1._addr.toString('hex') + '/name').set("Kiki's room");
	db.child('devices/' + rx3._addr.toString('hex') + '/name').set("Living room");

	rx1.on('data', process_signal).on('error', error_handler);
	rx3.on('data', process_signal).on('error', error_handler);

});



