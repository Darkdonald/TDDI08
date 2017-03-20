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
  sensitive << time_event;
}

void Monitor::monitor_method()
{
  *out << light_NS << " " << light_SN << " " << light_WE << " " << light_EW << "\n";
  counter++;
}

Monitor::~Monitor() {

}

void Monitor::end_of_simulation()
{
  delete out;
}

void Monitor::event_trigger_thread()
{
  for (;;) {               // Loop infinitely.
    wait(1, SC_SEC);       // Wait one second.
    time_event.notify();
  }
}
