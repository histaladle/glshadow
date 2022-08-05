#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

struct LightProperty {
    bool isEnable;
    bool isLocal;
    bool isSpot;
    vec3 ambient;
    vec3 color;
    vec3 position;
    vec3 halfVector;
    vec3 focus;
    float spotCosCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

    mat4 projection;
    mat4 view;
};

const int MAX_LIGHT=3;
uniform LightProperty qt_Lights[MAX_LIGHT];
uniform sampler2D qt_ShadowTextures[MAX_LIGHT];
uniform vec2 qt_ShadowTextureSizes[MAX_LIGHT];

uniform mat4 qt_Model;
uniform mat4 qt_View;
uniform mat4 qt_Projection;
uniform mat4 qt_LightModel;
uniform mat4 qt_LightView;
uniform mat4 qt_LightProjection;
varying vec4 vPos;
varying vec4 vColor;
uniform sampler2D qt_DepthTexture;
varying vec2 texCoord;
uniform mat4 qt_ShadowScaleBias;
void main(void)
{
    vec4 lightv;
    vec4 depthColor;
    vec2 shadowSize;
    LightProperty light;
    for(int i=0;i<MAX_LIGHT;i++)
    {
        light=qt_Lights[i];
        if(!light.isEnable)
        {
            continue;
        }
        lightv=qt_ShadowScaleBias*light.projection*light.view*qt_Model*vPos;

//        shadowSize=qt_ShadowTextureSizes[i];
//        if(shadowSize.x>1.0 && shadowSize.y>1.0)
//        {
//            for(int x=-1;x<=1;x++)
//            {
//                for(int y=-1;y<=1;y++)
//                {
//                    depthColor+=texture2DProj(
//                                qt_ShadowTextures[i],
//                                lightv+vec4(x/shadowSize.x,
//                                            y/shadowSize.y,0,0));
//                }
//            }
//            depthColor=depthColor/9;
//        }
//        else
//        {
//            depthColor=texture2DProj(qt_ShadowTextures[i],lightv);
//        }
        depthColor=texture2DProj(qt_ShadowTextures[i],lightv);
//        gl_FragColor=vec4(1.0)-depthColor;
        if(depthColor.r>0.0)
        {
            gl_FragColor=vec4(0.2,0.2,0.2,1.0);
            break;
        }
        else
        {
            gl_FragColor=vec4(0.8,0.8,0.8,1.0);
        }
    }

//    depthColor=texture2DProj(qt_DepthTexture,lightv);
//    gl_FragColor=vec4(1.0)-depthColor;
//    if(depthColor.r>0.2)
//    {
//        gl_FragColor=vec4(0.2,0.2,0.2,1.0);
//    }
//    else
//    {
//        gl_FragColor=vec4(0.8,0.8,0.8,1.0);
//    }


//    vec4 depthColor=texture2D(qt_DepthTexture,lightv.xy);
//    if(lightv.z>depthColor.r*lightv.w)
//    {
//        gl_FragColor=vec4(0.2,0.2,0.2,1.0);
//    }
//    else
//    {
//        gl_FragColor=vec4(0.8,0.8,0.8,1.0);
//    }

//    vec4 color=texture2D(qt_DepthTexture,texCoord);
//    gl_FragColor=color;
}
