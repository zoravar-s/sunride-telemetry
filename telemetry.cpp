#include <iostream>
#include <bitset>
#include <tuple>
#include <string>
#include <cstdint>
#include <cmath>
#include <chrono>

using namespace std;

auto start_time = std::chrono::high_resolution_clock::now();

std::tuple<bool, uint8_t, int8_t, float, uint16_t> encode(int detector, int barometer, int temperature, float acceleration, uint16_t elapsed_ms) {
    // error flag
    
    bool errorFlag = false;

    // initialize
    
    bool detector_conv = false;
    uint8_t barometer_conv = 0;
    float temperature_conv_temp = 0; // This allows for the conversion to be done in float before converting to int8_t
    int8_t temperature_conv = 0;
    int8_t  acceleration_conv = 0;

    // format detector input to boolean

    if (detector != 0 && detector != 23) {
        errorFlag = true;
    } else {
        detector_conv = (detector == 23); // conv to boolean
    }

    // format barometer input to unsigned 8 bit int (with pressure conversions)

    if (barometer < 0 || barometer > 255) {
        errorFlag = true;
    } else {
        // inversion and conversion factor/scaling applied:
        barometer_conv = (120-((barometer)/(255.0/(120-75))))/8.0; // conv to uint8_t
    }    

    if (temperature < 0 || temperature > 255) {
        errorFlag = true;
    } else {
        // conversion factor/scaling applied:
        temperature_conv_temp = (((temperature/(255.0/(150.0+10)))-10))/10.0;
        temperature_conv = round(temperature_conv_temp); // conv to int8_t
    } 

    if (acceleration < -256 || acceleration > 255) {
        errorFlag = true;
    } else {
        acceleration_conv = round(acceleration/16.0); // divided by 16 to save space, reconverted in the output
    } 

    uint16_t elapsed_conv = elapsed_ms / 10;

    if (errorFlag == true) {
        cout << errorFlag << endl;
        return make_tuple(false, 0, 0, 0, 0); // exit with error code
    } else {
        return make_tuple(detector_conv, barometer_conv, temperature_conv, acceleration_conv, elapsed_conv);
    }

    // return tuple of converted values


}

int main() {
    /*
    * maps telemetry with bool (As detector outputs only 2 possible values)
    * unsigned 8 bit int
    * signed 8 bit int
    * float (default 32 bit)
    */

    int detector;
    int barometer;
    int temperature;
    float acceleration;

    while (true) {

    cout << "Enter test data (detector, barometer, temperature, acceleration): ";
    cin >> detector >> barometer >> temperature >> acceleration;

    // encode the input values

    auto end_time = std::chrono::high_resolution_clock::now(); // get time since program start

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    auto [detector_conv, barometer_conv, temperature_conv, acceleration_conv, elapsed_conv] = encode(detector, barometer, temperature, acceleration, elapsed_ms);

    // output

    cout << "Truncated values: " << endl;
    cout << "Detector: " << detector_conv << endl;
    cout << "Barometer: " << static_cast<unsigned int>((barometer_conv)*8.0) << "kPa" << endl;
    cout << "Temperature: " << static_cast<int>((temperature_conv*10.0)) << " degrees C" << endl;
    cout << "Acceleration: " << acceleration_conv*16 << " m/s^2" << endl;
    
    cout << "Time elapsed: " << elapsed_conv*10 << " ms" << endl;

    cout << "4 byte send: " << std::bitset<16>(elapsed_conv) << std::bitset<1>(detector_conv) << std::bitset<4>(barometer_conv) << std::bitset<5>(temperature_conv) << std::bitset<6>(acceleration_conv) << endl;

    }

    return 0;
}