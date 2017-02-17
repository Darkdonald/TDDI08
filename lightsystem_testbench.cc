#include <systemc.h>
#include <time.h>
#include <fstream>
#include "lightsystem.h"
#include "input_gen.h"
#include "monitor.h"

int sc_main(int argc, char **argv)
{
  // The command-line arguments are as follows:
  // 1. the simulation time (in seconds),
  // 2. the file with input data (see input.txt), and
  // 3. the file to write output data.
  // 4. the file with prebuil data.
  // 5. Golden data for prebuild cases.
  assert(argc == 6);

  sc_time sim_time(atof(argv[1]), SC_SEC);
  char *infile = argv[2];
  char *outfile = argv[3];
  char *prefile = argv[4];
  char *goldenfile = argv[5];

  int testFailed = 0;

  // open infile stream
  ofstream *in;
  assert(infile != 0);
  in = new ofstream(infile);
  assert(*in);

  // open prefile stream
  ifstream *pre;
  assert(prefile != 0);
  pre = new ifstream(prefile);
  assert(*pre);

  // write pre-build to infile
  char buffer[10];
  while (pre->getline(buffer, 10)) {
    *in << buffer << "\n";
  }

  delete pre;

  // generate random input
  srand (time(NULL));
  for (int i = 0; i < 10; i++)  {
    *in << rand() % 2 << " " << rand() % 2 << " " << rand() % 2 << " " << rand() % 2 << "\n";
  }

  delete in;

  // Create channels.
  sc_signal<bool> sensor_NS_sig;
  sc_signal<bool> sensor_SN_sig;
  sc_signal<bool> sensor_WE_sig;
  sc_signal<bool> sensor_EW_sig;

  sc_signal<bool> light_NS_sig;
  sc_signal<bool> light_SN_sig;
  sc_signal<bool> light_WE_sig;
  sc_signal<bool> light_EW_sig;

  // Instantiate modules.
  Lightsystem lightsystem("Lightsystem");
  Generator gen("Generator", infile);
  Monitor monitor("Monitor", outfile);

  // Connect the channels to the ports.
  lightsystem(
    sensor_NS_sig,
    sensor_SN_sig,
    sensor_WE_sig,
    sensor_EW_sig,
    light_NS_sig,
    light_SN_sig,
    light_WE_sig,
    light_EW_sig
  );
  gen(
    sensor_NS_sig,
    sensor_SN_sig,
    sensor_WE_sig,
    sensor_EW_sig
  );
  monitor(
    sensor_NS_sig,
    sensor_SN_sig,
    sensor_WE_sig,
    sensor_EW_sig,
    light_NS_sig,
    light_SN_sig,
    light_WE_sig,
    light_EW_sig
  );

  // Start the simulation.
  sc_start(sim_time);

  // test-cases
  // 1) test independence with pre-build input
  // 2) SNNS-WEEW never green at the same time
  // 3) After a sensor imput the light must turn green in reasonable time

  if (testFailed) {
    std::cout << testFailed << " testcases failed!" << std::endl;
  } else {
    std::cout << "All testcases passed!" << std::endl;
  }

  return 0;
}
