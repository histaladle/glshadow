#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D qt_Texture;
varying vec2 texCoord;
varying vec4 vColor;
void main(void)
{
    gl_FragColor = texture2D(qt_Texture, texCoord);
}
