////http://nuclear.mutantstargoat.com/articles/sdr_fract/#mbrot_sdr
#version 410 core
#extension GL_ARB_gpu_shader_fp64 : enable

out vec4 FragColor;
uniform double time;
uniform dvec2 screenSize;
uniform dvec2 centre;
uniform double scale;
uniform int iter;
void main() {
    dvec2 uv = gl_FragCoord.xy / screenSize;
    //int iter = 150;
    dvec2 z, c;
    
    c.x = (uv.x - 0.5) * scale - centre.x;
    c.y = (uv.y - 0.5) * scale - centre.y;

    int i;
    z = c;
    for(i=0; i<iter; i++) {
        double x = (z.x * z.x - z.y * z.y) + c.x;
        double y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
        
    }
    if(i == iter){
        FragColor = vec4(0.0f, 0.0f, 0.0f,1.0f);
    }else{        
        //double colour = i*log(iter-1-i)/(1/scale*0.25)*log(float(length(z))*20)/50.0; //Scale
        double colour = i*log(iter-1-i)/log(float(length(z))*20)/50.0; //Scale
        FragColor = vec4(float(colour), float(colour*0.8), float((0.2-(colour*0.2))),1.0f);
    }
}