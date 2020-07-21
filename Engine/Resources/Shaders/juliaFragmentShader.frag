////http://nuclear.mutantstargoat.com/articles/sdr_fract/#mbrot_sdr
#version 410 core
out vec4 FragColor;
uniform vec2 screenSize;
uniform vec2 centre;
uniform float scale;
uniform vec2 translate;
uniform vec2 c;
void main() {
    //vec2 uv = vec2(gl_FragCoord.x/(planeScale-0.75f),gl_FragCoord.y/(planeScale+0.75f));
    vec2 uv = gl_FragCoord.xy/(screenSize*0.8);
    uv = gl_FragCoord.xy/(screenSize*0.8);
    int iter = 75;
    
    vec2 z;
    z.x = ((uv.x - 0.5)*8.0f - centre.x-0.5f-translate.x*4.5f);
    z.y = ((uv.y - 0.5)*8.0f - centre.y+0.5f-translate.y*6.8f);

    int i;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }
    if(i == iter){
        FragColor = vec4(0.0f, 0.0f, 0.0f,1.0f);
    }else{
        //float colour = i*log(iter-1-i)/log(60)/50.0f;
        float colour = i*log(iter-1-i)/log(length(z)*20)/50.0f;
        FragColor = vec4(colour, colour*0.8, 0.2-(colour*0.2),1.0f);
    }
}