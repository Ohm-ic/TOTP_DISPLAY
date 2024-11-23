Used For time based OTPs generation and to display it on the OLED Display in the form of QR Code. The TOTPs which are being genarated is at every 30 Seconds and the TOTP Would be generated at Every 1st Second
and at every 30th Second of a minute. The TOTPs are uing HMAC-SHA1 behind it. Also the codes(TOTPs) are same as what is being produced in the Google Authenticator is same Initial Key is Shared. 
The OLED Display is an oled display module 1.3" 128x64 i2c with SH1106 Driver IC. Just connect the SDA and SCL pins of the Display to Pin number 21 and 22 of the ESP32, Give your SSID and Password, Share an Initial Key String
and let it do it's work.


Also if you are using an Smaller 0.96" Display having SSD1306 Driver IC just Change the Library #include "SH1106Wire.h" to #include "SSD1306Wire.h" and it will run properly.


For Query, Feedback, Suggestions feel free to Contact- hariomagrahari06@gmail.com

Cheers
