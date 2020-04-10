# Mpp-PZEM-V3-
PZEM V3 device for AutomationManagerIOT https://play.google.com/store/apps/details?id=mpp.android.wemo
The speech about accurate and non-invasive Energy-meter developed for Mpp implementations.
The device might be used as additional counter of energy , as additional monitor of power loading and also as "warning-device". Having quite impressive sensivity and accuracy- 0.1V , 0.001A, 0.1W and 0.1Hz it has several advantages like- 
non-invasivness - you don't need interfere with existing wiring, good price , good reliabilty and broad rangeI use to using it as additional energy monitor for my country home and for friends country homes and garages and that more important as "warning-device". It can warn you in case of exceeding power level , changing in electricity properties , unwillingly electricity leakage or being forgotten powered on devices.

The heart and brain of the device is EPS8266 based Whemos D1 and PZEM V3 module . There is only one more device is needed - cheap 5V power source.  You can also purchase a version with split-core transformer, that is more convenient in installation but also more expensive.
The device measures voltage (80-280V),current(0-100A), active power(0-23kW), power factor(0-1), frequency(45-65Hz) and active energy(0-9999.99kWth)  the accuracy noted above and send the data to AM server. Working temperature (-20C- +60C).

How it works... You need connect high voltage input( phase and NL) of PZEM to any suitable power connector  and thread a phase wire into the current ring of the PZEM. Being seeked of the device in AM you will see 5 analog devices :
DefaultUDN+"V", DefaultUDN+"I"  ,DefaultUDN+"P",  DefaultUDN+"E",  DefaultUDN+"F" , i have omitted power factor but it is no difficulties to get it back.
I decided to use software serial connection to PZEM hence GPIO 12 and 14 are used for this connection and classic TX and RX are left for usuall monitoring or program downloading  if any demanded...

Classically i have to warn you that any works with high voltage are dangerous and has to be done by only whom understand something in it.

I have noticed that mostly i'm using it as a "warning-device" - i have virtual AM device HomeAlarm - that is flag which intended to switch on several watсhdog devices when i leaving my country home. Hence i can establish the rule if the idle power is exceeding the common value of it - AM warns me that some device is forgotten in on state and consume the power. Also the one of the intention of these devices is power control service for electrical line of heating convectors.
Due to restriction on energy consumption for particular hours i have to limit most big consumer like heater-convector in particular time therefore if the system sees "overload" it swithes off some of the convectors till the suitable energy values.

One more , like disclaimer...-  The TTL level of PZEM is 5V , TTL level of WEMOS is 3.3V and in a good way there has to be TTL level convertor betwwen them , but usually it works without any convertors ,because output level on PZEM TTL is not more 4V , due to resistors on ESP , WEMOS is tolerant for such voltage. As the proof the first device have been working for more than two years without  any troubles.
