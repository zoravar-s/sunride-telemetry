#include <iostream>
#include <bitset>
#include <tuple>
#include <string>
#include <cstdint>
#include <cmath>
#include <chrono>

using namespace std;

int decodeSignedField(uint32_t value, unsigned int width) {
    const uint32_t signBit = 1U << (width - 1);
    if (value & signBit) {
        return static_cast<int>(value) - static_cast<int>(1U << width);
    }
    return static_cast<int>(value);
}

std::tuple<string, string, string, string, string> decode(const string& binaryString) {

    // check if the binary string is valid
    if (binaryString.length() != 32) {
        return make_tuple("0", "0", "0", "0", "0"); // exit with error code
    }

    // get first 16 characters of the binary string
    string first16 = binaryString.substr(0, 16);
    // convert the first 16 characters to an unsigned integer
    uint16_t time = bitset<16>(first16).to_ulong();
    time = time * 10;

    // get next character
    char detector_char = binaryString[16];
    // convert the character to 23 if true, 0 if false
    bool detector_bool = (detector_char == '1');
    int detector = 0;
    if (detector_bool) {
        detector = 23;
    } else {
        detector = 0;
    }

    // get next 4 characters of the binary string
    string next4 = binaryString.substr(17, 4);
    // convert the next 4 characters to an integer
    int barometer = bitset<4>(next4).to_ulong();
    barometer = barometer * 8;

    // get next 5 characters of the binary string
    string next5 = binaryString.substr(21, 5);
    // decode
    int temperature = decodeSignedField(static_cast<uint32_t>(bitset<5>(next5).to_ulong()), 5);
    temperature = (temperature*10);

    // get next 6 characters of the binary string
    string next6 = binaryString.substr(26, 6);
    // decode
    int acceleration = decodeSignedField(static_cast<uint32_t>(bitset<6>(next6).to_ulong()), 6);
    acceleration = acceleration * 16;

    // return the decoded values as a tuple
    return make_tuple(to_string(time), to_string(detector), to_string(barometer), to_string(temperature), to_string(acceleration));
    


}

int main() {
    /*
    * maps telemetry with bool (As detector outputs only 2 possible values)
    * unsigned 8 bit int
    * signed 8 bit int
    * float (default 32 bit)
    */

    string binaryString;

    while (true) {

    cout << "Enter binary string:  ";
    cin >> binaryString;

    // encode the input values

    auto [time_conv, detector_conv, barometer_conv, temperature_conv, acceleration_conv] = decode(binaryString);
    

    // output

    cout << "Decoded values: " << endl;
    cout << "Detector: " << detector_conv << endl;
    cout << "Barometer: " << barometer_conv << "kPa" << endl;
    cout << "Temperature: " << temperature_conv << " degrees C" << endl;
    cout << "Acceleration: " << acceleration_conv << " m/s^2" << endl;
    cout << "Time elapsed: " << time_conv << " ms" << endl;

    }

    return 0;
}