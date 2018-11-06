#include <iostream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

int main()
{
    //输入图像
   GDALDataset* poSrcDS;
   //输出图像
   GDALDataset* poDstDS;
   //图像的宽度和高度
   int imgXlen,imgYlen;
   //起始位置坐标，白色和黑色
   int StartX1 = 300,StartY1 = 300;
   int StartX2 = 500,StartY2 = 500;
   //区域宽度和高度，白色黑色
   int tmpXlen1 = 200,tmpYlen1 = 50;
   int tmpXlen2 = 50,tmpYlen2 = 100;
   //输入图像路径
   char* srcPath = "may.jpg";
   //输出图像路径
   char* dstPath = "res.tif";
   //图像内存存储
   GByte* buffTmp;
   //图像波段数
   int i,j,bandNum,k;
   //注册驱动
   GDALAllRegister();
   //打开图像
   poSrcDS = (GDALDataset*)GDALOpenShared(srcPath,GA_ReadOnly);
    //获取图像的宽度，高度和波段数
   imgXlen = poSrcDS->GetRasterXSize();
   imgYlen = poSrcDS->GetRasterYSize();
   bandNum = poSrcDS->GetRasterCount();

   //根据图像的宽度和高度分配内存
   buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen*sizeof(GByte));
//创建输出图像
   poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
     dstPath,imgXlen,imgYlen,bandNum,GDT_Byte,NULL);
//free
//将图片输出到新建的图片
       for(i = 0;i < bandNum;i ++)
   {
       poSrcDS->GetRasterBand(i + 1)->RasterIO(
        GF_Read,0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);
       poDstDS->GetRasterBand(i + 1)->RasterIO(
        GF_Write,0,0,imgXlen,imgYlen,buffTmp,imgXlen,imgYlen,GDT_Byte,0,0);


   }
CPLFree(buffTmp);
   //分配白色区域内存
buffTmp = (GByte*)CPLMalloc(tmpXlen1*tmpYlen1*sizeof(GByte));

for(k = 1;k <= 3;k ++)
{
    //依次获取通道1 2 3缓存在bufftmp
   poSrcDS->GetRasterBand(k)->RasterIO(GF_Read,StartX1,StartY1,tmpXlen1,tmpYlen1,buffTmp,tmpXlen1,tmpYlen1,GDT_Byte,0,0);
   //遍历区域，将新建图片该区域改变颜色
   for(j = 0;j < tmpYlen1;j ++)
    for(i = 0;i < tmpXlen1;i ++)
   {
       //置255
       buffTmp[j * tmpXlen1  +i] = (GByte)255;
   }
   poDstDS->GetRasterBand(k)->RasterIO(GF_Write,StartX1,StartY1,tmpXlen1,tmpYlen1,buffTmp,tmpXlen1,tmpYlen1,GDT_Byte,0,0);
}
CPLFree(buffTmp);
   //分配黑色区域内存
   buffTmp = (GByte*)CPLMalloc(tmpXlen2*tmpYlen2*sizeof(GByte));
   //读取红色通道缓存在buffTmp中，只是为了获取该区域
   for(k = 1;k <= 3;k ++)
   {
       //依次获得1,2,3通道
       poSrcDS->GetRasterBand(k)->RasterIO(GF_Read,StartX2,StartY2,tmpXlen2,tmpYlen2,buffTmp,tmpXlen2,tmpYlen2,GDT_Byte,0,0);

       for(j = 0;j < tmpYlen2;j ++)
        for(i = 0;i < tmpXlen2;i ++)
       {
           //置0
           buffTmp[j * tmpXlen2  +i] = (GByte)0;
       }
       poDstDS->GetRasterBand(k)->RasterIO(GF_Write,StartX2,StartY2,tmpXlen2,tmpYlen2,buffTmp,tmpXlen2,tmpYlen2,GDT_Byte,0,0);
   }

   //清除内存
   CPLFree(buffTmp);
   //关闭dataset
   GDALClose(poDstDS);
   GDALClose(poSrcDS);
   system("PAUSE");
   return 0;
}
