#include "../../base/Engine.h"
#include "glog/logging.h"
#include "StencilTest.h"

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::WARNING);

    Engine engine("stencil_test", true);
    engine.setRenderer(std::make_shared<StencilTest>());
    engine.execute();
}
