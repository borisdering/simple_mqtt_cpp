let some = require('../index');

let client = some.connect('mqtt://localhost:1883');

client.subscribe("#");

client.on('message', (message) => {
    console.log(`did receive a message ${message}`);
});