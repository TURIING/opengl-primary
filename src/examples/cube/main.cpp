#include <iostream>
#include "../../base/Engine.h"
#include "glog/logging.h"
#include "../../primitive/Cube.h"

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);

    Engine engine("cube", true);
    engine.setRenderer(std::make_shared<Cube>());
    engine.execute();
}
