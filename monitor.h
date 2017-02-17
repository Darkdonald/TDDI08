#ifndef MONITOR_H
#define MONITOR_H

#include <systemc.h>
#include <fstream>

using std::ofstream;

SC_MODULE(Monitor) {
  sc_in<bool> sensor_NS;
  sc_in<bool> sensor_SN;
  sc_in<bool> sensor_WE;
  sc_in<bool> sensor_EW;

  sc_in<bool> light_NS;
  sc_in<bool> light_SN;
  sc_in<bool> light_WE;
  sc_in<bool> light_EW;

  sc_event time_event;

  int counter = 0;

  SC_HAS_PROCESS(Monitor);
  Monitor(sc_module_name name, char *outfile);
  ~Monitor();

  void monitor_method();
  void check_constraints_method();
  void event_trigger_thread();

  ofstream *out;
};

#endif // MONITOR_H
