#ifndef INPUT_GEN_H
#define INPUT_GEN_H

#include <systemc.h>
#include <fstream>

using std::ifstream;

SC_MODULE(Generator) {
  sc_out<bool> sensor_NS;
  sc_out<bool> sensor_SN;
  sc_out<bool> sensor_WE;
  sc_out<bool> sensor_EW;

  SC_HAS_PROCESS(Generator);
  Generator(sc_module_name name, char *datafile);
  ~Generator();

  int counter = 0;

  void generate_thread();

  ifstream *in;
};

#endif // INPUT_GEN_H
