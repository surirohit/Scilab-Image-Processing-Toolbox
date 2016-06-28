#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc opencv_imread;
extern Gatefunc opencv_detectHarrisFeatures;
extern Gatefunc opencv_detectMinEigenFeatures;
extern Gatefunc opencv_corner;
extern Gatefunc opencv_ocr;
extern Gatefunc opencv_imageSet;
extern Gatefunc opencv_partition;
extern Gatefunc opencv_trainImageCategoryClassifier;
extern Gatefunc opencv_bagOfFeatures;
extern Gatefunc opencv_predict;
extern Gatefunc opencv_evaluate;
extern Gatefunc opencv_detectCheckerboardPoints;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imread,"opencv_imread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_detectHarrisFeatures,"opencv_detectHarrisFeatures"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_detectMinEigenFeatures,"opencv_detectMinEigenFeatures"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_corner,"opencv_corner"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_ocr,"opencv_ocr"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imageSet,"opencv_imageSet"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_partition,"opencv_partition"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_trainImageCategoryClassifier,"opencv_trainImageCategoryClassifier"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_bagOfFeatures,"opencv_bagOfFeatures"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_predict,"opencv_predict"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_evaluate,"opencv_evaluate"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_detectCheckerboardPoints,"opencv_detectCheckerboardPoints"},
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
