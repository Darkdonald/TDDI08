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

  int simTime = atof(argv[1]);

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

  int counter = 0;
  // write pre-build to infile
  char buffer[10];
  while (pre->getline(buffer, 10)) {
    counter++;
    *in << buffer << "\n";
  }

  delete pre;

  // generate random input
  srand (time(NULL));
  for (int i = 0; i < (simTime - counter - simTime/4); i++)  {
    *in << rand() % 2 << " " << rand() % 2 << " " << rand() % 2 << " " << rand() % 2 << "\n";
  }
  *in << 0 << " " << 0 << " " << 0 << " " << 0 << "\n";

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
  sc_stop();

  // test-cases
  // 1) test independence with pre-build input

  bool test_NS = 0;
  bool test_SN = 0;
  bool test_WE = 0;
  bool test_EW = 0;
  bool pre_NS = 0;
  bool pre_SN = 0;
  bool pre_WE = 0;
  bool pre_EW = 0;

  // open prefile stream
  ifstream *gold;
  gold = new ifstream(goldenfile);

  // open outfile stream
  ifstream *out;
  out = new ifstream(outfile);
  assert(*out);

  char goldBuffer[10];
  char testBuffer[10];

  while (gold->getline(goldBuffer, 10)) {
    out->getline(testBuffer, 10);

    for (int i = 0; i < 7; i++) {
      if (goldBuffer[i] != testBuffer[i]) {
        testFailed = 1;
      }
    }
  }

  delete gold;
  delete out;

  // 2) SNNS-WEEW never green at the same time
  out = new ifstream(outfile);

  bool light_NS, light_SN, light_WE, light_EW;

  while(*out) {
    *out >> light_NS >> light_SN >> light_WE >> light_EW;
    if ((light_NS == 1 || light_SN == 1) && (light_WE == 1 || light_EW == 1)) {
      testFailed = 1;
    }
  }

  delete out;

  // 3) After a sensor imput the light must turn green in reasonable time
  out = new ifstream(outfile);
  ifstream *input;
  input = new ifstream(infile);

  bool car[4];
  bool light[4];

  int lastCar [4] = {0, 0, 0, 0};
  int lastLight [4] = {0, 0, 0, 0};

  counter = 0;

  while(*input) {
    *input >> car[0] >> car[1] >> car[2] >> car[3];

    for (int i = 0; i < 4; i++) {
      if (car[i] == 1) {
        lastCar[i] = counter;
      }
    }

    counter++;
  }

  counter = 0;

  while(*out) {
    *out >> light[0] >> light[1] >> light[2] >> light[3];

    for (int i = 0; i < 4; i++) {
      if (light[i] == 1) {
        lastLight[i] = counter;
      }
    }

    counter++;
  }

  for (int i = 0; i < 4; i++) {
    if (lastLight[i] < lastCar[i]) {
      testFailed = 1;
    }
  }

  delete out;

  if (testFailed) {
    std::cout << "\nTestcases failed!" << std::endl;
  } else {
    std::cout << "\nAll testcases passed!" << std::endl;
  }

  return 0;
}
