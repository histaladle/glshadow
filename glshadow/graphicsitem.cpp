#include "graphicsitem.h"

GraphicsItem::GraphicsItem()
{
}

void GraphicsItem
::draw(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
{
    Q_UNUSED(projection);
    Q_UNUSED(view);
    Q_UNUSED(model);
    if(!vBuffer.isCreated())
    {
        return;
    }
    if(!inited)
    {
        return;
    }
}

void GraphicsItem::updateBuffer()
{
}

void GraphicsItem::setLight(int index, LightProperty light)
{
    Q_UNUSED(index);
    Q_UNUSED(light);
}

void GraphicsItem
::renderDepth(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
{
    Q_UNUSED(projection);
    Q_UNUSED(view);
    Q_UNUSED(model);
}

void GraphicsItem
::renderId(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
{
    Q_UNUSED(projection);
    Q_UNUSED(view);
    Q_UNUSED(model);
}

GraphicsItem::~GraphicsItem()
{

}
