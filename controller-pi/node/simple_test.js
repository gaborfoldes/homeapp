var nrf = require('nrf');

var spiDev = "/dev/spidev0.0";
var cePin = 24;
var irqPin = 25;

var radio = nrf.connect(spiDev, cePin, irqPin); // Connect to the radio
radio.channel(0x4c); // Set channel to 76
radio.dataRate('1Mbps') // Set data rate to 1Mbps
radio.crcBytes(2) // Set the CRC to 2
radio.autoRetransmit({
	count: 15,
	delay: 4000
}); // Auto retransmit up to 15 times


function process_signal (d) {
	c = d.readUInt32BE(0)/100;
	signal = { timestamp: new Date().toJSON(),
		   device: {
		     id: this._addr.toString('hex')
		   },
		   data: {
		     celsius: c,
		     fahrenheit: (c * 9 / 5 ) + 32
		   }
		 };
	console.log(signal);
}

function error_handler (e) {
	console.log("Error:", e);
}
 
 
// Start the radio
radio.begin(function() {
 
	var rx1 = radio.openPipe('rx', 0xF0F0F0F0E1); // Listen at address 
	var rx3 = radio.openPipe('rx', 0xF0F0F0F0E3); // Listen at address 

	rx1.on('data', process_signal).on('error', error_handler);
	rx3.on('data', process_signal).on('error', error_handler);

});

//	rx3.on('ready', function() {
//		console.log("RX Ready:", rx3._addr.toString('hex')); //readUInt8(0));
//		radio.printDetails();
//	});

//	rx1.on('data', function(d) {
//		signal = { Celsius: d.readUInt32BE(0)/100 };
//		console.log(signal);
//		console.log("Living room -- sensor type:", d.readUInt32BE(4), "Celsius:", d.readUInt32BE(0)/100); 
//	}).on('error', function(e) {
//		console.log("Error:", e);
//	});


