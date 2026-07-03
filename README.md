***Flick*** <br>
An Automated light switch flipper, paired with Orion virtual assistant using MQTT (Message Querying Telemetry Transport).

Made with an Esp32, an SG90 servo, and 3D printed brackets. Mounted onto a household lever-style light switch via tucked in folds.

-------------------------------------------------------------------------------------------------------------------------
On Boot, ESP32 logs into respective wifi network and subscribes to the orion/flick topic, waiting for a message. Checks callback functions for "on" or "off" and executes servo positions accordingly.
