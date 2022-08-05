#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 qt_Vertex;
attribute vec3 qt_Color;
varying vec4 vColor;
attribute vec3 qt_Normal;
varying vec4 fNormal;
varying vec4 vPos;
uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;
void main(void)
{
    vPos=vec4(qt_Vertex,1.0);
    vColor=vec4(qt_Color,1.0);
    fNormal=vec4(qt_Normal,1.0);
    gl_Position=qt_Projection*qt_View*qt_Model*vPos;
}
