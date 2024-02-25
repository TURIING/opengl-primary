#include <iostream>
#include "../../base/Engine.h"
#include "glog/logging.h"
#include "../../primitive/Triangle.h"

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::WARNING);

    Engine engine("triangle", false);
    engine.setRenderer(std::make_shared<Triangle>());
    engine.execute();
}
