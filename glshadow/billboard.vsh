#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
attribute vec3 qt_Vertex;
varying vec2 texCoord;
attribute vec2 qt_TexCoord;
uniform mat4 qt_Model;
uniform mat4 qt_Projection;
uniform mat4 qt_View;
varying vec4 fragPosition;
uniform vec3 qt_Center;
uniform vec3 qt_EyePosition;
varying vec4 vColor;
uniform float width,height;
void main(void)
{
    vec4 look,right,up,center,eye,pos;

    center = vec4(qt_Center,1.0);
    center = qt_View*qt_Model*center;
    eye = vec4(qt_EyePosition,1.0);

    look = eye-center;
    look = vec4(look.xyz,1.0);

    right = vec4(cross(vec3(0.0,1.0,0.0),look.xyz),1.0);
    right = vec4(normalize(right.xyz),1.0);

    up = vec4(cross(look.xyz,right.xyz),1.0);
    up = vec4(normalize(up.xyz),1.0);

    pos = center+(qt_Vertex.x-qt_Center.x)*right
            +(qt_Vertex.y-qt_Center.y)*up;
    pos = vec4(pos.xyz,1.0);
    fragPosition = pos;
    gl_Position = qt_Projection*pos;
//    gl_Position = qt_Projection*qt_ModelView*qt_Vertex;
    texCoord=qt_TexCoord;
}
