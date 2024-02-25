#include <iostream>
#include "../../base/Engine.h"
#include "glog/logging.h"
#include "ModelLoad.h"
#include "../../BaseDefine.h"

const auto modelPath = std::string(MODEL_PATH) + "/nanosuit/nanosuit.obj";

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);

    Engine engine("model_load", true);
    engine.setRenderer(std::make_shared<ModelLoad>(modelPath));
    engine.execute();
}
