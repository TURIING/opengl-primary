#include "../../base/Engine.h"
#include "glog/logging.h"
#include "FrameBufferDemo.h"

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::WARNING);

    Engine engine("FrameBufferDemo", true);
    engine.setRenderer(std::make_shared<FrameBufferDemo>());
    engine.execute();
}
