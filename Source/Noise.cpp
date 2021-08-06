#include<algorithm>
#include<cstdio>
#include<ctime>
#include<cmath>

#define uint8_t unsigned char

//https://github.com/nothings/stb
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

using namespace std;

const int random_limit(128);
const int image_length(512);

inline double get_value()
{
    return (rand()%(random_limit+1))/(double)random_limit;
}

inline double fade(double t)
{
    return (t=1-t)*0+t*t*t*(t*(6*t-15)+10);
}

inline void set_color(bool use_color,uint8_t height,uint8_t *color)
{
    if(!use_color)
    {
        color[0]=color[1]=color[2]=height;
        return;
    }
    if(0<=height&&height<=128)
    {
        color[1]=height*2;
        color[0]=255-color[1];
        color[2]=0;
    }else if(129<=height&&height<=255)
    {
        color[0]=0;
        color[1]=(height-127)*2;
        color[2]=255-color[1];
    }
}

void generate_noise(int *ways,int ways_count,uint8_t *pimage)
{
    double sum_weight(0);
    double *image=new double[image_length*image_length*3];
    for(int i=0;i<=(image_length*image_length*3)-1;i+=1)
        image[i]=0.0;
    for(int i=0;i<=ways_count-1;i+=1)
        sum_weight+=ways[(i<<1)|1];
    for(int i=0;i<=ways_count-1;i+=1)
    {
        int block_length=ways[i<<1];
        int block_weight=ways[(i<<1)|1];
        int block_counts=image_length/block_length+2;
        double *block_values=new double[block_counts*block_counts];
        for(int j=0;j<=(block_counts*block_counts)-1;j+=1)
            block_values[j]=get_value();
        int index=0;
        for(int x=0;x<=image_length-1;x+=1)
        {
            for(int y=0;y<=image_length-1;y+=1)
            {
                int xx=x/block_length+1;
                int yy=y/block_length+1;
                double u=fade((y%block_length+1)/(double)(block_length+1));
                double v=fade((x%block_length+1)/(double)(block_length+1));
                double a=block_values[(xx-1)*block_counts+yy-1];
                double b=block_values[(xx-1)*block_counts+yy];
                double c=block_values[(xx)*block_counts+yy-1];
                double d=block_values[(xx)*block_counts+yy];
                double tt=a*u+b*(1-u);
                double dd=c*u+d*(1-u);
                double res=tt*v+dd*(1-v);
                res*=block_weight/sum_weight;
                for(int c=1;c<=3;c+=1)
                    image[index++]+=res;
            }
        }
        delete[] block_values;
    }
    for(int i=0;i<=(image_length*image_length*3)-1;i+=3)
        set_color(false,(uint8_t)(image[i]*255),&(pimage[i]));
    delete[] image;
}

int main()
{
    srand((unsigned int)2317);  
    srand((unsigned int)76312312);
    uint8_t *image=new uint8_t[image_length*image_length*3];
    int ways[]{128,8,64,7,32,6,16,5,8,4,4,3,2,2,1,1};
    generate_noise(ways,sizeof(ways)/(2*sizeof(int)),image);
    stbi_write_jpg("Noise.jpg",image_length,image_length,3,image,100);
    delete[] image;
    return 0;
}