#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 qt_Vertex;
attribute vec4 qt_Color;
varying vec4 vColor;
attribute vec4 qt_Normal;
varying vec4 fNormal;
varying vec4 vPos;
uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;
attribute vec2 qt_TexCoord;
varying vec2 texCoord;
void main(void)
{
    vPos=vec4(qt_Vertex,1.0);
    vColor=qt_Color;
    fNormal=qt_Normal;
    gl_Position=qt_Projection*qt_View*qt_Model*vPos;
    texCoord=qt_TexCoord;
}
