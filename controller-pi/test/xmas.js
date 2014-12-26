
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

//var Firebase = require('firebase');
//var db = new Firebase('https://vivid-fire-6554.firebaseio.com/');


// Start the radio

radio.begin(function() {
 
	console.log('Starting radio...');

	var tx1 = radio.openPipe('tx', 0xF0F0F0F0D2, {autoAck:false});
	tx1.on('ready', function (d) {
            var buf = new Buffer(4);
            buf.writeUInt32BE(1, 0);
            tx1.write(buf);
            console.log('...signal sent');
	});

});



