#include "../../base/Engine.h"
#include "glog/logging.h"
#include "Blend.h"

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::WARNING);

    Engine engine("Blend", true);
    engine.setRenderer(std::make_shared<Blend>());
    engine.execute();
}
