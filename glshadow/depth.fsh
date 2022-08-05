#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec4 vPos;
uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;

void main(void)
{
    vec4 v=qt_Projection*qt_View*qt_Model*vPos;
    v=vec4(vec3(v.z/v.w),1.0);
    gl_FragColor = v;
}
