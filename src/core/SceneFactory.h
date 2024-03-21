/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/21 22:00
* @version: 1.0
* @description: 利用工厂模式动态创建场景类
********************************************************************************/

#ifndef OPENGL_PRIMARY_SCENEFACTORY_H
#define OPENGL_PRIMARY_SCENEFACTORY_H

#include <string>
#include <functional>
#include <memory>
#include "IScene.h"
#include "glog/logging.h"

class SceneFactory {
public:
    static SceneFactory* instance() {
        static SceneFactory factory;
        return &factory;
    }

    std::shared_ptr<IScene> makeScene(std::string &_name) {
        LOG_ASSERT(!m_map.empty());

        auto it = m_map.find(_name);
        if(it != m_map.end()) {
            return it->second();
        }

        LOG(ERROR) << " Attempt to generate invalid scene class";
    }

    std::shared_ptr<IScene> makeInitialScene() {
        LOG_ASSERT(!m_map.empty());

        return m_map.begin()->second();
    }

    // 注册场景类
    template<class SceneType>
    void registerScene() {
        m_map[std::string(typeid(SceneType).name())] = []() {
            return std::make_shared<SceneType>();
        };
    }

    std::vector<std::string> getSceneNameList() {
        std::vector<std::string> result;
        for(const auto &[name, _]: m_map) {
            result.push_back(name);
        }
        return result;
    }
private:
    SceneFactory() { }
private:
    std::map<std::string, std::function<std::shared_ptr<IScene>()>> m_map;
};
#endif //OPENGL_PRIMARY_SCENEFACTORY_H
