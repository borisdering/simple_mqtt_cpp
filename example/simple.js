let { 
    mosquitto_init, 
    mosquitto_loop, 
    mosquitto_on_message, 
    mosquitto_subscribe 
} = require('../index');

mosquitto_on_message((message) => {
    console.log(`message called in simple js`);
});
mosquitto_init();

mosquitto_subscribe("#");


setInterval(() => {
    mosquitto_loop();
}, 10);