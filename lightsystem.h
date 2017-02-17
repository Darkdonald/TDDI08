#ifndef LIGHTSYSTEM_H
#define LIGHTSYSTEM_H

#include <systemc.h>

SC_MODULE(Lightsystem) {
  sc_in<bool> sensor_NS;
  sc_in<bool> sensor_SN;
  sc_in<bool> sensor_WE;
  sc_in<bool> sensor_EW;

  sc_out<bool> light_NS;
  sc_out<bool> light_SN;
  sc_out<bool> light_WE;
  sc_out<bool> light_EW;

  sc_event time_event;

  int light[4] = {0,0,0,0};
  int car[4] = {0,0,0,0};
  int sensor[4] = {0,0,0,0};
  int count[4] = {0,0,0,0};

  int NSSN = 0;
  int WEEW = 0;

  SC_HAS_PROCESS(Lightsystem);
  Lightsystem(sc_module_name name);

  void control_lights_method();
  void event_trigger_thread();
  void setLight();
  int sumArray(int* array);
};

#endif // LIGHTSYSTEM_H
