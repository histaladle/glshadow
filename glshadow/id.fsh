#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform int qt_ObjectId;
uniform mat4 qt_Projection;
uniform mat4 qt_View;
uniform mat4 qt_Model;

varying vec4 vPos;
void main(void)
{
//    vec4 color=vec4(float(qt_ObjectId&0xff)/255.0,
//                    float((qt_ObjectId>>8)&0xff)/255.0,
//                    float((qt_ObjectId>>16)&0xff)/255.0,
//                    float((qt_ObjectId>>24)&0xff)/255.0);
    vec4 color;
    color.x=float(qt_ObjectId-qt_ObjectId/255*255)/255.0;
    color.y=float(qt_ObjectId/255-qt_ObjectId/255/255*255)/255.0;
    color.z=float(qt_ObjectId/255/255-qt_ObjectId/255/255/255*255)/255.0;
    gl_FragColor=color;
}
