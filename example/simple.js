let some = require('../index');

let client = some.connect('mqtt://localhost:1883');

client.subscribe("#");


    // client.publish('topic2', 'message', null, () => {
    //     console.log('done publish');
    // });

client.on('message', (topic, message) => {
    console.log(`1: topic: '${topic.toString()}' message: '${message.toString()}'`);
    // client.publish('topic2', 'message', null, () => {
    //     console.log('done publish');
    // });
});
