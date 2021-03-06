
var radio = require('nrf').connect("/dev/spidev0.0", 24, 25);  //TODO: add IRQ pin
radio.channel(0x4c).transmitPower('PA_HIGH').dataRate('1Mbps').crcBytes(2).autoRetransmit({count:15, delay:15});
radio.begin(function () {
    console.log('Starting radio...');
    var rx3 = radio.openPipe('rx', 0xF0F0F0F0E3),
        rx1 = radio.openPipe('rx', 0xF0F0F0F0E1);
    //rx.pipe(tx);        // echo back everything
    //rx.on('ready', function() {
    //    console.log(rx);
    //});
    rx3.on('data', function (d) {
        console.log("Got data on #3:", d);
    });
    rx1.on('data', function (d) {
        console.log("Got data on #1:", d);
    });
});
