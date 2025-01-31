#ifndef __DAVAENGINE_DRAWABLE_H__
#define __DAVAENGINE_DRAWABLE_H__

#include "Base/BaseObject.h"
#include "Base/BaseTypes.h"
#include "Base/BaseMath.h"
#include "Render/RenderBase.h"
#include "Scene3D/SceneNode.h"

namespace DAVA
{
class Drawable
{
public:
    /**
     * @brief Pure virtual method to draw the object
     */
    virtual void Draw() = 0;

    /**
     * @brief Pure virtual method to get sort identifier
     * @return Sorting identifier for the drawable
     */
    virtual uint64 GetSortID() = 0;
};
};

#endif // __DAVAENGINE_SCENENODE_H__
