const {     
    mosquitto_init, 
    mosquitto_loop, 
    mosquitto_on_message, 
    mosquitto_subscribe,
    mosquitto_connect,
    mosquitto_publish
} = require('./build/Release/mosquittopp.node');
const url = require('url');

const connect = (href) => {

    let location = url.parse(href);

    console.log(location);

    mosquitto_init();
    mosquitto_connect(location.hostname, Number(location.port));

    mosquitto_loop();
    setInterval(() => {
        mosquitto_loop();
    }, 10);

    return {
        on: (event, callback = () => {}) => {
            switch (event) {
                case 'connect':
                    callback();
                    break;

                case 'message':
                    mosquitto_on_message(callback);
                    break;
            
                default:
                    break;
            }
        },
        publish: (topic, message, options, callback = () => {}) => {
            mosquitto_publish(topic, message, options.retain);
            callback();
        },
        subscribe: (topic, options, callback = () => {}) => {
            mosquitto_subscribe(topic);
            callback();
        }
    };
};

 module.exports = {  
    connect
 };