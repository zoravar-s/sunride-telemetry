# sunride-telemetry
Sunride application telemetry program using 4 bytes of memory to assign/decode.<br>
Some data loss is apparent to fit into 4 bytes - timestamp allows up to 11 minutes of flight time, acceleration allows for -256 to 255 m/s^2 to be read on-board. Any unsuitable values read will result in the outputted data being blank.
