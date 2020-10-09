let some = require('../index');

let client = some.connect('mqtt://localhost:1883');

client.subscribe("#");


    // client.publish('topic2', 'message', null, () => {
    //     console.log('done publish');
    // });

client.on('message', (message) => {
    console.log(`0: did receive a message ${message.toString()}`);
    // client.publish('topic2', 'message', null, () => {
    //     console.log('done publish');
    // });
});

client.on('message', (message) => {
    console.log(`1: did receive a message ${message.toString()}`);
    // client.publish('topic2', 'message', null, () => {
    //     console.log('done publish');
    // });
});

client.on('message', (message) => {
    console.log(`2: did receive a message ${message.toString()}`);
    // client.publish('topic2', 'message', null, () => {
    //     console.log('done publish');
    // });
});