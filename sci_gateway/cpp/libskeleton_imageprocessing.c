#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc opencv_imread;
extern Gatefunc opencv_imageSet;
extern Gatefunc opencv_partition;
extern Gatefunc opencv_trainImageCategoryClassifier;
extern Gatefunc opencv_bagOfFeatures;
extern Gatefunc opencv_imageCategoryClassifier;
extern Gatefunc opencv_evaluate;
extern Gatefunc opencv_detectCheckerboardPoints;
extern Gatefunc opencv_ForegroundDetector;
extern Gatefunc opencv_VideoReader;
extern Gatefunc has_frame;
extern Gatefunc read_frame;
extern Gatefunc opencv_readForegroundMask;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imread,"imread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imageSet,"imageSet"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_partition,"partition"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_trainImageCategoryClassifier,"trainImageCategoryClassifier"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_bagOfFeatures,"bagOfFeatures"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imageCategoryClassifier,"imageCategoryClassifier"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_evaluate,"evaluate"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_detectCheckerboardPoints,"detectCheckerboardPoints"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_ForegroundDetector,"ForegroundDetector"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_VideoReader,"VideoReader"},
  {(Myinterfun)sci_gateway_without_putlhsvar,has_frame,"hasFrame"},
  {(Myinterfun)sci_gateway_without_putlhsvar,read_frame,"readFrame"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_readForegroundMask,"readForegroundMask"},
};
 
int C2F(libskeleton_imageprocessing)()
{
  Rhs = Max(0, Rhs);
  if (*(Tab[Fin-1].f) != NULL) 
  {
     if(pvApiCtx == NULL)
     {
       pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
     }
     pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  }
  return 0;
}
#ifdef __cplusplus
}
#endif
