let { 
    mosquitto_init, 
    mosquitto_loop, 
    mosquitto_on_message, 
    mosquitto_subscribe,
    mosquitto_connect 
} = require('../index');

mosquitto_on_message((message) => {
    console.log(`message called in simple js ${message}`);
});
mosquitto_init();

mosquitto_connect("localhost", 1883);

mosquitto_subscribe("#");


setInterval(() => {
    mosquitto_loop();
}, 10);