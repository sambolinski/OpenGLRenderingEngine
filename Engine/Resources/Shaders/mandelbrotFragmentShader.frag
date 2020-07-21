////http://nuclear.mutantstargoat.com/articles/sdr_fract/#mbrot_sdr
#version 410 core
out vec4 FragColor;
uniform float time;
uniform vec2 screenSize;
uniform vec2 centre;
uniform float scale;
void main() {
    vec2 uv = gl_FragCoord.xy / screenSize;
    int iter = 100;
    vec2 z, c;
    
    c.x = (uv.x - 0.5) * scale - centre.x;
    c.y = (uv.y - 0.5) * scale - centre.y;

    int i;
    z = c;
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