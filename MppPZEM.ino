#include <Arduino.h>
#include <ESP.h>
#include <ESP8266WiFi.h>
//#include <SoftwareSerial.h>
#include <MppServer.h>
#include <MppDevice.h>
#include <MppHTTPCLient.h>
#include <stdio.h>
#include <PZEM004Tv30.h>


const char *DeviceVersion = "MppPZEM 1.0.1";
static const char *P_PERIOD = "Period"; // Period - the frequency to read the analog input in sconds
static const char *RESET_E="RESET ENERGY";

// properties handled by MppSensor
static const char *properties[] = { //
        P_PERIOD, //
        RESET_E,
        NULL };
MppServer mppServer(DeviceVersion, properties);
class MppDevice volt;
class MppDevice curr;
class MppDevice powr;
class MppDevice ener;
class MppDevice freq;

unsigned long next = millis();

// Use software serial for the PZEM
PZEM004Tv30 pzem(12,14);

void setup() {
  Serial.begin(115200);
    // set a default of GPIO0 (typical) for recovery
  mppServer.setPropertyDefault(P_BUTTON_PIN, "0");
  mppServer.setPropertyDefault(RESET_E, "false");
  mppServer.setPropertyDefault(P_PERIOD, "1000");

  mppServer.manageDevice(&volt, getDefaultUDN(MppAnalog)+"V");
  mppServer.manageDevice(&curr, getDefaultUDN(MppAnalog)+"I");
  mppServer.manageDevice(&powr, getDefaultUDN(MppAnalog)+"P");
  mppServer.manageDevice(&ener, getDefaultUDN(MppAnalog)+"E");
  mppServer.manageDevice(&freq, getDefaultUDN(MppAnalog)+"F");


  Serial.printf("\nMppServer booting: %s, mode=%d, version=%d\n",
      ESP.getResetReason().c_str(), ESP.getBootMode(),
      ESP.getBootVersion());

      mppServer.begin();
}

void loop() {
   bool notify;
  unsigned long now = millis();
  float v,i,p,e,f;

  mppServer.handleClients(); // let the server handle any incoming requests
  mppServer.handleCommand(); // optional, handle user Serial input 
  
  if (now > next) {
if(mppServer.isProperty(RESET_E)) {
  Serial.printf("Energy counter is being resetted\n");
  pzem.resetEnergy();
  mppServer.putProperty(RESET_E, "false");
}
    v = pzem.voltage();
    if( !isnan(v) ){
        Serial.print("Voltage: "); Serial.print(v); Serial.println("V");
        notify=volt.put(VALUE,String(v)); notify=volt.put(STATE,"on"); volt.update(VALUE,String(v)); 
    } else {
        Serial.println("Error reading voltage");
        notify=volt.put(STATE,"off");
    }

    i = pzem.current();
    if( !isnan(i) ){
        notify=curr.put(VALUE,String(i)); notify=curr.put(STATE,"on");
        Serial.print("Current: "); Serial.print(i); Serial.println("A");
    } else {
        Serial.println("Error reading current");
        notify=curr.put(STATE,"off");
    }

    p = pzem.power();
    if( !isnan(p) ){
        notify=powr.put(VALUE,String(p)); notify=powr.put(STATE,"on");
        Serial.print("Power: "); Serial.print(p); Serial.println("W");
    } else {
        notify=powr.put(STATE,"off");
        Serial.println("Error reading power");
    }

    e= pzem.energy();
    if( !isnan(e) ){
        notify=ener.put(VALUE,String(e)); notify=ener.put(STATE,"on");
        Serial.print("Energy: "); Serial.print(e,3); Serial.println("kWh");
    } else {
        notify=ener.put(STATE,"off");
        Serial.println("Error reading energy");
    }

    f = pzem.frequency();
    if( !isnan(f) ){
        notify=freq.put(VALUE,String(f)); notify=freq.put(STATE,"on");
        Serial.print("Frequency: "); Serial.print(f, 1); Serial.println("Hz");
    } else {
        notify=freq.put(STATE,"off");
        Serial.println("Error reading frequency");
    }

    float pf = pzem.pf();
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }

    Serial.println();

 next = now + mppServer.getUnsignedProperty(P_PERIOD);
  }
}
