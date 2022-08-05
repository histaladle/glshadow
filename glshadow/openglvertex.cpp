#include "openglvertex.h"

OpenGLVertex::OpenGLVertex()
{

}

bool OpenGLVertex::operator ==(const OpenGLVertex other) const
{
    bool matched=true;
    matched&=(pos==other.pos);
    matched&=(normal==other.normal);
    matched&=(texCoord==other.texCoord);
    return matched;
}
