#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
attribute vec4 qt_Vertex;
attribute vec4 qt_Color;
attribute vec3 qt_Normal;
varying vec3 fragNormal;
varying vec4 fragPosition;
varying vec4 vColor;
varying vec2 texCoord;
attribute vec2 qt_TexCoord;
//attribute highp vec4 qt_MultiTexCoord0;
//uniform mat4 qt_ModelViewProjection;
uniform mat4 qt_ModelView;
uniform mat4 qt_Projection;
//varying highp vec4 qt_TexCoord0;
//uniform mat4 matrix;
//varying vec4 vertex;
void main(void)
{
//    color = texture2D(qt_Texture, qt_TexCoord);
    vColor = qt_Color;
    fragNormal = qt_Normal;
    texCoord = qt_TexCoord;
    gl_Position = qt_Projection*qt_ModelView*qt_Vertex;
    fragPosition = qt_ModelView*qt_Vertex;
//    qt_TexCoord0 = qt_MultiTexCoord0;
//    color = vColor;
//    gl_Position = matrix * vPosition;
}
