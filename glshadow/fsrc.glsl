//uniform sampler2D qt_Texture0;
//varying highp vec4 qt_TexCoord0;
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
    vec3 coneDirection;
    float spotCosCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

const int MAX_LIGHT=10;

uniform LightProperties qt_Lights[MAX_LIGHT];

//uniform float qt_Shininess;
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

//uniform vec3 qt_EyeDirection;
uniform vec3 qt_EyePosition;
varying vec4 fragPosition;
varying vec4 vColor;
varying vec3 fragNormal;

varying vec2 texCoord;
uniform sampler2D qt_Texture;
uniform int qt_FragType;

void main(void)
{
//    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
//    gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//    gl_FragColor = color;
    if(qt_FragType==0)
    {
        gl_FragColor = vColor;
    }
    else if(qt_FragType==1)
    {
        gl_FragColor = texture2D(qt_Texture, texCoord);
    }
    else if(qt_FragType==2 || qt_FragType==3)
    {
        vec4 texColor;
        if(qt_FragType==2)
        {
            texColor = texture2D(qt_Texture, texCoord);
        }
        else
        {
            texColor = vColor;
        }

        vec3 normal=vec3(qt_NormalMatrix*vec4(fragNormal,1.0));
        normal=normalize(normal);
        if(!gl_FrontFacing)
        {
            normal=-normal;
        }
        vec3 eyeDirection=qt_EyePosition-fragPosition.xyz;
        eyeDirection=normalize(eyeDirection);

//        normal=fragNormal;
        vec3 scatteredLight=vec3(0.0);
        vec3 reflectedLight=vec3(0.0);

        for(int i=0;i<MAX_LIGHT;i ++)
        {
            if(!qt_Lights[i].isEnable)
            {
                continue;
            }
            vec3 halfVector;
            vec3 lightDirection=qt_Lights[i].position;
            float attenuation=1.0;

            if(qt_Lights[i].isLocal)
            {
                lightDirection=lightDirection-vec3(fragPosition);
                float lightDistance=length(lightDirection);
                lightDirection=lightDirection/lightDistance;
                attenuation=1.0/(qt_Lights[i].constantAttenuation
                                 +qt_Lights[i].linearAttenuation
                                 *lightDistance
                                 +qt_Lights[i].quadraticAttenuation
                                 *lightDistance*lightDistance);
                if(qt_Lights[i].isSpot)
                {
                    float spotCos=dot(lightDirection,
                                      -qt_Lights[i].coneDirection);
//                    gl_FragColor=vec4(spotCos*0.05);
//                    break;
                    if(spotCos<qt_Lights[i].spotCosCutoff)
                    {
                        attenuation=0.0;
                    }
                    else
                    {
                        attenuation*=pow(spotCos,
                                         qt_Lights[i].spotExponent);
                    }
                    halfVector=normalize(lightDirection
                                         +eyeDirection);
                }
                else
                {
//                    halfVector=qt_Lights[i].halfVector;
                    halfVector=normalize(qt_Lights[i].position
                                         +eyeDirection);
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
                scatteredLight+=qt_Lights[i].ambient
                        *Material.ambient*attenuation
                        +qt_Lights[i].color*Material.diffuse
                        *diffuse*attenuation;
                reflectedLight+=qt_Lights[i].color
                        *Material.specular*specular*attenuation;
            }
            vec3 rgb=min(Material.emission
                         +texColor.rgb*scatteredLight+reflectedLight,
                         vec3(1.0));
            gl_FragColor=vec4(rgb,texColor.a);
        }
    }
    else
    {
        gl_FragColor = vec4(0.5, 0.5, 0.5, 0.5);
    }
}
