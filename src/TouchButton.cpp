// #include <TouchButton.h>
// #include <Config.h>
// #include <Display.h>
// #include <Colors.h>
// #include <WString.h>

// #define BTN1_PIN 13
// #define BTN2_PIN 12
// #define BTN3_PIN 14

// ICACHE_RAM_ATTR void btn1_process() 
// {
//     btn1_status = true;
//     Serial.print("BTN 1 pressed.");
// }

// ICACHE_RAM_ATTR void btn2_process() 
// {
//     btn2_status = true;
//     Serial.print("BTN 2 pressed.");
// }

// ICACHE_RAM_ATTR void btn3_process()
// {
//     btn3_status = true;
//     Serial.print("BTN 3 pressed.");
// }

// void TouchButton::setup() 
// {
//     pinMode(BTN1_PIN, INPUT);
//     pinMode(BTN2_PIN, INPUT);
//     pinMode(BTN3_PIN, INPUT);

//     attachInterrupt(digitalPinToInterrupt(BTN1_PIN), btn1_process, RISING);
//     attachInterrupt(digitalPinToInterrupt(BTN2_PIN), btn2_process, RISING);
//     attachInterrupt(digitalPinToInterrupt(BTN3_PIN), btn3_process, RISING);
// }   

// void TouchButton::loop() 
// {    
//     if (btn1_status) {
//         btn1_status = false;
//     } else if (btn2_status) {
//         btn2_status = false;
//     } else if (btn3_status) {
//         btn3_status = false;
//     }
// }