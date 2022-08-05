#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

struct LightProperties {
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

uniform LightProperties qt_Lights[MAX_LIGHT];
uniform sampler2D qt_ShadowTextures[MAX_LIGHT];
uniform vec2 qt_ShadowTextureSizes[MAX_LIGHT];
uniform float qt_Strength;

struct MaterialProperties {
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess; // 0 ~ 1000
};

uniform MaterialProperties Material;

uniform mat4 qt_NormalMatrix;


uniform mat4 qt_Projection;
uniform mat4 qt_View;
uniform mat4 qt_Model;

varying vec4 vColor;
varying vec4 fNormal;
varying vec4 vPos;
uniform mat4 qt_ShadowScaleBias;
void main(void)
{
    vec4 lightPrj;
    vec4 depthColor=vec4(0.0);
    vec2 shadowSize;
    float shadowFac;
    vec3 lightFragDirection;
    bool lightEyeDirectionSame;

    vec4 fPos=/*qt_View**/qt_Model*vPos;
    vec4 texColor;
    texColor=vColor;
    vec3 normal=vec3(qt_NormalMatrix*fNormal);
    normal=normalize(normal);
    if(!gl_FrontFacing)
    {
        normal=-normal;
    }
    vec3 eyePos=vec3(qt_View[3][0],qt_View[3][1],qt_View[3][2]);
    vec3 eyeDirection=eyePos/*-vec3(fPos)*/;
    eyeDirection=normalize(eyeDirection);

    vec3 scatteredLight=vec3(0.0);
    vec3 reflectedLight=vec3(0.0);

    bool existLight=false;
    for(int i=0;i<MAX_LIGHT;i++)
    {
        existLight=existLight||qt_Lights[i].isEnable;
        if(!qt_Lights[i].isEnable)
        {
            continue;
        }
        /*shadow*/
        lightPrj=qt_Lights[i].projection*qt_Lights[i].view*qt_Model*vPos;
        vec4 shadowuv=qt_ShadowScaleBias*lightPrj;
        depthColor=texture2DProj(qt_ShadowTextures[i],shadowuv);
        shadowFac=lightPrj.z-depthColor.r*lightPrj.w;
        if(lightPrj.w>0.0)
        {
            if(shadowFac>0.001*lightPrj.w)
            {
                shadowFac=0.4;
            }
            else
            {
                shadowFac=0.0;
            }
        }
        else
        {
            shadowFac=0.0;
        }

//        if(lightPrj.w>0.0)
//        {
//            vec2 shadowuv=lightPrj.xy/lightPrj.w*0.5+0.5;
//            depthColor=texture2D(qt_ShadowTextures[i],shadowuv);
//            shadowFac=lightPrj.z-(depthColor.r)*lightPrj.w;
//            if(shadowFac>0.001*lightPrj.w)
//            {
//                shadowFac=0.4;
//            }
//            else
//            {
//                shadowFac=0.0;
//            }
//        }
//        else
//        {
//            shadowFac=0.0;
//        }


        /*failed*/
//        shadowSize=qt_ShadowTextureSizes[i];
//        depthColor=vec4(0.0);
//        if(shadowSize.x>1.0 && shadowSize.y>1.0)
//        {
//            for(int x=-1;x<=1;x++)
//            {
//                for(int y=-1;y<=1;y++)
//                {
//                    depthColor+=texture2DProj(
//                                qt_ShadowTextures[i],
//                                fv+vec4(x/shadowSize.x,y/shadowSize.y,0,0));
//                }
//            }
//            depthColor=depthColor/9;
//        }
//        else
//        {
//            depthColor=texture2DProj(qt_ShadowTextures[i],fv);
//        }
//        lightFragDirection=qt_Lights[i].position-vec3(fPos);
//        lightFragDirection=normalize(lightFragDirection);

//        lightEyeDirectionSame=dot(cross(lightFragDirection,fNormal.xyz),
//                                  cross(eyeDirection,fNormal.xyz))>0;
//        lightEyeDirectionSame=dot(lightFragDirection,fNormal.xyz)
//                *dot(eyeDirection,fNormal.xyz)>0;
//        if(abs(lightPrjUv.x)<abs(lightPrjUv.w)
//                && abs(lightPrjUv.y)<abs(lightPrjUv.w)
//                && lightEyeDirectionSame)
//        {
//            shadowFac=(lightPrjUv.p-(depthColor.r)*lightPrjUv.q);
//            if(lightPrjUv.p-(depthColor.r)*lightPrjUv.q>0.2)
//            {
//                shadowFac=0.4;
//            }
//            else
//            {
//                shadowFac=0.0;
//            }
//        }
//        else
//        {
//            shadowFac=0.0;
//        }
//        float bias=max(0.01*(1.0-dot(normal,-lightFragDirection)),0.1);
//        shadowFac=lightPrj.z-(depthColor.r)*lightPrj.w;
//        if(shadowFac<0.0)
//        {
//            shadowFac=0.0;
//        }
//        else if(shadowFac>0.9)
//        {
//            shadowFac=0.0;
//        }
//        else if(shadowFac>0.1)
//        {
//            shadowFac=0.4;
//        }
//        else
//        {
//            shadowFac=0.0;
//        }

        vec3 halfVector;
        vec3 lightDirection=qt_Lights[i].position;
        float attenuation=1.0;
        vec3 coneDirection=qt_Lights[i].focus-qt_Lights[i].position;
        coneDirection=normalize(coneDirection);
        if(qt_Lights[i].isLocal)
        {
            lightDirection=lightDirection-vec3(fPos);
            float lightDistance=length(lightDirection);
            lightDirection=lightDirection/lightDistance;
            attenuation=1.0/(qt_Lights[i].constantAttenuation
                             +qt_Lights[i].linearAttenuation
                             *lightDistance
                             +qt_Lights[i].quadraticAttenuation
                             *lightDistance*lightDistance);
            if(qt_Lights[i].isSpot)
            {
                float spotCos=dot(lightDirection,-coneDirection);
                if(spotCos<qt_Lights[i].spotCosCutoff)
                {
                    attenuation=0.0;
                }
                else
                {
                    attenuation*=pow(spotCos,
                                     qt_Lights[i].spotExponent);
                }
                halfVector=normalize(lightDirection+eyeDirection);
            }
            float diffuse=max(0.0,dot(normal,lightDirection));
            float specular=max(0.0,dot(normal,halfVector));

            if(diffuse==0.0)
            {
                specular=0.0;
            }
            else
            {
                specular=pow(specular,Material.shininess)*qt_Strength;
            }
//            scatteredLight+=qt_Lights[i].ambient
//                    *Material.ambient*attenuation
//                    +qt_Lights[i].color*Material.diffuse
//                    *diffuse*attenuation;
            scatteredLight+=(qt_Lights[i].ambient
                    *Material.ambient*attenuation
                    +qt_Lights[i].color*Material.diffuse
                    *diffuse*attenuation)*(1.0-shadowFac);
//            reflectedLight+=qt_Lights[i].color
//                    *Material.specular*specular*attenuation;
            reflectedLight+=(qt_Lights[i].color
                    *Material.specular*specular*attenuation)
                    *(1.0-shadowFac);
        }
        vec3 rgb=min(Material.emission
                     +texColor.rgb*scatteredLight+reflectedLight,
                     vec3(1.0));
        gl_FragColor=vec4(rgb,texColor.a);
    }

    if(!existLight)
    {
        gl_FragColor=texColor;
    }
}
