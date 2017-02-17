#include <cassert>
#include "monitor.h"

Monitor::Monitor(sc_module_name name, char *outfile)
  : sc_module(name)
{
  assert(outfile != 0);
  out = new ofstream(outfile);
  assert(*out);

  SC_THREAD(event_trigger_thread);

  SC_METHOD(monitor_method);
  dont_initialize();
  sensitive << time_event;
}

Monitor::~Monitor()
{
  delete out;
}

void Monitor::monitor_method()
{
  *out << "counter: " << counter << "\n" << sensor_NS << sensor_SN << sensor_WE << sensor_EW << "\n" << light_NS << light_SN << light_WE << light_EW << "\n  \n";
  counter++;
}

void Monitor::event_trigger_thread()
{
  for (;;) {               // Loop infinitely.
    wait(1, SC_SEC);       // Wait one second.
    time_event.notify();
  }
}
