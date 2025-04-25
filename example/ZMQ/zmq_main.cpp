#include "infinite_sense.h"
#include <thread>
using namespace infinite_sense;
int main() {
  Synchronizer synchronizer;
  // use net link
  // synchronizer.SetNetLink("192.168.1.188", 8888);
  // use serial link
  synchronizer.SetSerialLink("/dev/ttyACM0",460800);
  // use mv camera
  // synchronizer.UseMvCam();
  synchronizer.Start();
  Synchronizer::PrintSummary();
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds{1000});
  }
  synchronizer.Stop();
  return 0;
}