# sunride-telemetry
Sunride application telemetry program using 4 bytes of memory to assign/decode.<br>
Some data loss is apparent to fit into 4 bytes - timestamp allows up to 11 minutes of flight time, acceleration allows for -256 to 255 m/s^2 to be read on-board. Any unsuitable values read will result in the outputted data being blank.
Both the encoder and decoder are only one subroutine. They are split into different apps for ease of use.
The instructions were a bit confusing TBH. I assumed the sensors output a linear scale from 0 to 255 which is then converted to the range of the instruments (and decreasing in reading for the barometer).
"Flight time" is assumed to be the time since the "telemetry" app was started.
