#include <Rcpp.h>
#include <boost/shared_ptr.hpp>
#include <R_ext/GraphicsEngine.h>

typedef boost::shared_ptr< std::vector< std::string > > GrafflePtr;

using namespace Rcpp;

std::string i2s(int val){
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

std::string d2s(double val){
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

class GraffleDevice {

public:

  GrafflePtr graffle_script;

  GraffleDevice(int height, int width) {
    Rcout << "cnvs = document.windows[0].selection.canvas;" << std::endl;
    Rcout << "cnvs.size = new Size(" + i2s(height) + "," + i2s(width) + ");" << std::endl;
  }

};


double image_strwidth(const char *str, pGEcontext gc, pDevDesc dd) {
  BEGIN_RCPP
  return(0);
  VOID_END_RCPP
  return(0);
}

void image_metric_info(int c, pGEcontext gc, double* ascent, double* descent,
                       double* width, pDevDesc dd) {
  Rcout << "// image_metric_info()" << std::endl;
}

static void image_close(pDevDesc dd) {
  Rcout << "// image_close()" << std::endl;
  BEGIN_RCPP
  VOID_END_RCPP
}

static void image_clip(double left, double right, double bottom, double top, pDevDesc dd) {
  Rcout << "// image_clip()" << std::endl;
  BEGIN_RCPP
  VOID_END_RCPP
}

static void image_size(double *left, double *right, double *bottom, double *top, pDevDesc dd) {
  Rcout << "// image_size()" << std::endl;
  *left = dd->left;
  *right = dd->right;
  *bottom = dd->bottom;
  *top = dd->top;
}

static void image_new_page(const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_new_page()" << std::endl;
  BEGIN_RCPP
  VOID_END_RCPP
}

static void image_line(double x1, double y1, double x2, double y2, const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_line()" << std::endl;
  BEGIN_RCPP
  GraffleDevice *gd = (GraffleDevice *)dd->deviceSpecific;
  std::string new_line = "line = ";
  new_line = new_line + "cnvs.addLine(" +
    "new Point(" + d2s(x1) + "," + d2s(y1) + ")" + ", " +
    "new Point(" + d2s(x2) + "," + d2s(y2) + ")" + ");\n";
  new_line = new_line + "line.shadowColor = null;";
  Rcout << new_line << std::endl;
  VOID_END_RCPP
}

static void image_polyline(int n, double *x, double *y, const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_polyline()" << std::endl;
  BEGIN_RCPP
  std::string new_line = "line = cnvs.newLine();\n";
  new_line = new_line + "line.points = [\n";
  for (int i=0; i<n; i++) {
    new_line = new_line + "  new Point(" + d2s(x[i]) + "," + d2s(y[i]) + ")";
    if (i < (n-1)) new_line = new_line + ", ";
  }
  new_line = new_line + "\n];\n";
  new_line = new_line + "line.shadowColor = null;";
  Rcout << new_line << std::endl;
  VOID_END_RCPP
}

static void image_polygon(int n, double *x, double *y, const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_polygon()" << std::endl;
  BEGIN_RCPP
  VOID_END_RCPP
}

void image_text(double x, double y, const char *str, double rot, double hadj,
                pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_text()" << std::endl;
  BEGIN_RCPP
  GraffleDevice *gd = (GraffleDevice *)dd->deviceSpecific;
  std::string new_text = "t1 = ";
  new_text = new_text + "cnvs.addText(\"" + std::string(str) + "\", " +
    "new Point(" + d2s(x) + "," + d2s(y) + ")" + ");\n";
  new_text = new_text + "t1.shadowColor = null;";
  Rcout << new_text << std::endl;
  VOID_END_RCPP
}

static void image_rect(double x0, double y0, double x1, double y1,
                       const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_rect()" << std::endl;
  BEGIN_RCPP
  VOID_END_RCPP
}

static void image_circle(double x, double y, double r, const pGEcontext gc, pDevDesc dd) {

  Rcout << "// image_circle()" << std::endl;

  std::string new_circle = "c1 = cnvs.newShape();\n";
  new_circle += "c1.shape = \"Circle\";\n";
  new_circle += "c1.shadowColor = null;\n";
  new_circle += "c1.geometry = new Rect(" + d2s(x-r) + ", " + d2s(y-r) + ", " + d2s(2*r) + ", " + d2s(2*r) + ");";

  Rcout << new_circle << std::endl;

  BEGIN_RCPP
  //note: parameter 3 + 4 must denote any point on the circle
  VOID_END_RCPP
}

static void image_path(double *x, double *y, int npoly, int *nper, Rboolean winding,
                       const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_path()" << std::endl;
  BEGIN_RCPP
  VOID_END_RCPP
}

void image_mode(int mode, pDevDesc dd) {
  Rcout << "// image_mode()" << std::endl;
}

SEXP image_capture(pDevDesc dd){
  Rcout << "// image_capture()" << std::endl;
  BEGIN_RCPP
  return R_NilValue;
  VOID_END_RCPP
  return R_NilValue;
}


static void image_raster(unsigned int *raster, int w, int h,
                double x, double y,
                double width, double height,
                double rot,
                Rboolean interpolate,
                const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_raster()" << std::endl;
  BEGIN_RCPP
  VOID_END_RCPP
}

static pDevDesc graffle_driver_new(GraffleDevice *device, int bg, int width, int height,
                                   double ps, int res, int canclip) {

  int res0 = (res > 0) ? res : 72;

  // initialization
  pDevDesc dd = (DevDesc*) calloc(1, sizeof(DevDesc));
  dd->startfill = bg;
  dd->startcol = R_RGB(0, 0, 0);
  dd->startps = ps;
  dd->startlty = 0;
  dd->startfont = 1;
  dd->startgamma = 1;

  // callbacks
  dd->activate = NULL;
  dd->deactivate = NULL;
  dd->close = image_close;
  dd->clip = image_clip;
  dd->size = image_size;
  dd->newPage = image_new_page;
  dd->line = image_line;
  dd->text = image_text;
  dd->strWidth = image_strwidth;
  dd->rect = image_rect;
  dd->circle = image_circle;
  dd->polygon = image_polygon;
  dd->polyline = image_polyline;
  dd->path = image_path;
  dd->mode = image_mode;
  dd->metricInfo = image_metric_info;
  dd->cap = image_capture;
  dd->raster = image_raster;

  // Converts text() with fontface = 5 (adobe symbols) to UTF-8, see also BMDeviceDriver
#ifdef __APPLE__
  dd->wantSymbolUTF8 = TRUE;
#else
  dd->wantSymbolUTF8 = FALSE;
#endif

  dd->hasTextUTF8 = (Rboolean) 1;
  dd->textUTF8 = image_text;
  dd->strWidthUTF8 = image_strwidth;

  // Screen Dimensions in pts
  dd->left = 0;
  dd->top = 0;
  dd->right = width;
  dd->bottom = height;

  // Magic constants copied from other graphics devices
  // nominal character sizes in pts
  dd->cra[0] = 0.9 * ps * res0/72.0;
  dd->cra[1] = 1.2 * ps * res0/72.0;

  // character alignment offsets
  dd->xCharOffset = 0.4900;
  dd->yCharOffset = 0.3333;
  dd->yLineBias = 0.2;
  // inches per pt
  dd->ipr[0] = 1.0 / res0;
  dd->ipr[1] = 1.0 / res0;

  // Capabilities
  dd->canClip = (Rboolean)canclip;
  dd->canHAdj = 0;
  dd->canChangeGamma = FALSE;
  dd->displayListOn = FALSE;
  dd->haveLocator = FALSE;
  dd->haveTransparency = 2;
  dd->haveTransparentBg = 2;
  dd->haveRaster = 2;
  dd->haveCapture = 2;
  dd->deviceSpecific = device;

  return(dd);

}

static void make_device(GraffleDevice *device, std::string bg_,
                        int width, int height,
                        double pointsize, int res, bool canclip) {

  int bg = R_GE_str2col(bg_.c_str());

  R_GE_checkVersionOrDie(R_GE_version);
  R_CheckDeviceAvailable();

  BEGIN_SUSPEND_INTERRUPTS {

    pDevDesc dev = graffle_driver_new(device, bg, width, height, pointsize, res, canclip);

    if (dev == NULL)  throw std::runtime_error("Failed to start graffle device");

    pGEDevDesc dd = GEcreateDevDesc(dev);

    GEaddDevice2(dd, "graffle");

    GEinitDisplayList(dd);

  } END_SUSPEND_INTERRUPTS;

}

// [[Rcpp::export]]
std::string graffle_device_internal(std::string bg, int width, int height, double pointsize,
                                 int res, bool clip, bool antialias, bool drawing) {

  GraffleDevice *device = new GraffleDevice(width, height);

  make_device(device, bg, width, height, pointsize, res, clip);

  return "";

}
