/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/20 22:08
* @version: 1.0
* @description: 审查面板类
********************************************************************************/

#ifndef OPENGL_PRIMARY_INSPECTPANEL_H
#define OPENGL_PRIMARY_INSPECTPANEL_H

#include "IPanel.h"
#include <memory>

class IPrimitive;
class IMaterial;

class InspectPanel: public IPanel {
public:
    void render() override;

    void dispatch(Event _event, EventParam _param) override;

private:
    void onPrimitiveSelected(EventParam &_param);
    void buildMaterialItem(std::shared_ptr<IMaterial> _material);
    void buildItemForPhongMaterial(std::shared_ptr<IMaterial> &_material);
    void buildItemForSkyboxMaterial(std::shared_ptr<IMaterial> &_material);
    std::shared_ptr<IMaterial> makeMaterialByType(MaterialType _type);

private:
    std::shared_ptr<IPrimitive> m_currentPrimitive = nullptr;
};


#endif //OPENGL_PRIMARY_INSPECTPANEL_H
