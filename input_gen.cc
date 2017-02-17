#include "input_gen.h"
#include <cassert>

Generator::Generator(sc_module_name name, char *datafile)
  : sc_module(name)
{
  assert(datafile != 0);       // An input file should be given.

  in = new ifstream(datafile); // Open the input file.
  assert(*in);                 // Check that everything is OK.

  SC_THREAD(generate_thread);

  sensor_NS.initialize(0);
  sensor_SN.initialize(0);
  sensor_WE.initialize(0);
  sensor_EW.initialize(0);
}

Generator::~Generator()
{
  delete in;
}

void Generator::generate_thread()
{
  bool sen_NS, sen_SN, sen_WE, sen_EW;
  for (;;) {
    wait(1, SC_SEC);     // Generate new inputs every second.
    *in >> sen_NS >> sen_SN >> sen_WE >> sen_EW; // Read from the input file.
    sensor_NS->write(sen_NS);
    sensor_SN->write(sen_SN);
    sensor_WE->write(sen_WE);
    sensor_EW->write(sen_EW);
    cout << "\n" << "counter: " << counter << "\n";
    counter++;
  }
}
