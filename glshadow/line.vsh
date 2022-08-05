#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec3 qt_Vertex;
attribute vec4 qt_Color;
uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;
varying vec4 vColor;
varying vec4 vPos;
void main(void)
{
    vPos=vec4(qt_Vertex,1.0);
    vColor=qt_Color;
    gl_Position=qt_Projection*qt_View*qt_Model*vPos;
}
