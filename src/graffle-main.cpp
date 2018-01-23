#include <Rcpp.h>
#include <gdtools.h>
#include <boost/shared_ptr.hpp>
#include <R_ext/GraphicsEngine.h>

// from 'svglite' source: 1 lwd = 1/96", but units in rest of document are 1/72"
#define xlwd (72.0/96.0)

typedef boost::shared_ptr< std::vector< std::string > > GrafflePtr;

std::string normalize_font(const char * family);

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
  Rcpp::List system_aliases;
  Rcpp::List user_aliases;
  XPtrCairoContext cc;

  GraffleDevice(int height, int width, Rcpp::List aliases_):
    system_aliases(Rcpp::wrap(aliases_["system"])),
    user_aliases(Rcpp::wrap(aliases_["user"])),
    cc(gdtools::context_create()) {
    std::string new_canvas = "cnvs = document.windows[0].selection.canvas;\n";
    new_canvas = new_canvas +
      tfm::format("cnvs.size = new Size(%d, %d);", i2s(height), i2s(width));
    Rcout << new_canvas << std::endl;
  }

};


inline bool is_black(int col) {
  return (R_RED(col) == 0) && (R_GREEN(col) == 0) && (R_BLUE(col) == 0) &&
    (R_ALPHA(col) == 255);
}

inline bool is_filled(int col) {
  return R_ALPHA(col) != 0;
}

inline bool is_bold(int face) {
  return face == 2 || face == 4;
}
inline bool is_italic(int face) {
  return face == 3 || face == 4;
}
inline bool is_bolditalic(int face) {
  return face == 4;
}
inline bool is_symbol(int face) {
  return face == 5;
}

inline std::string find_alias_field(std::string& family, Rcpp::List& alias,
                                    const char* face, const char* field) {
  if (alias.containsElementNamed(face)) {
    Rcpp::List font = alias[face];
    if (font.containsElementNamed(field))
      return font[field];
  }
  return std::string();
}

inline std::string find_user_alias(std::string& family,
                                   Rcpp::List const& aliases,
                                   int face, const char* field) {
  std::string out;
  if (aliases.containsElementNamed(family.c_str())) {
    Rcpp::List alias = aliases[family];
    if (is_bolditalic(face))
      out = find_alias_field(family, alias, "bolditalic", field);
    else if (is_bold(face))
      out = find_alias_field(family, alias, "bold", field);
    else if (is_italic(face))
      out = find_alias_field(family, alias, "italic", field);
    else if (is_symbol(face))
      out = find_alias_field(family, alias, "symbol", field);
    else
      out = find_alias_field(family, alias, "plain", field);
  }
  return out;
}

inline std::string find_system_alias(std::string& family,
                                     Rcpp::List const& aliases) {
  std::string out;
  if (aliases.containsElementNamed(family.c_str())) {
    SEXP alias = aliases[family];
    if (TYPEOF(alias) == STRSXP && Rf_length(alias) == 1)
      out = Rcpp::as<std::string>(alias);
  }
  return out;
}

inline std::string fontname(const char* family_, int face,
                            Rcpp::List const& system_aliases,
                            Rcpp::List const& user_aliases) {
  std::string family(family_);
  if (face == 5)
    family = "symbol";
  else if (family == "")
    family = "sans";

  std::string alias = find_system_alias(family, system_aliases);
  if (!alias.size())
    alias = find_user_alias(family, user_aliases, face, "name");

  if (alias.size())
    return alias;
  else
    return family;
}

inline std::string fontfile(const char* family_, int face,
                            Rcpp::List user_aliases) {
  std::string family(family_);
  if (face == 5)
    family = "symbol";
  else if (family == "")
    family = "sans";

  return find_user_alias(family, user_aliases, face, "file");
}


double image_strwidth(const char *str, pGEcontext gc, pDevDesc dd) {

  Rcout << "// image_strwidth()" << std::endl;
  BEGIN_RCPP
  GraffleDevice *gd = (GraffleDevice *)dd->deviceSpecific;

  std::string file = fontfile(gc->fontfamily, gc->fontface, gd->user_aliases);
  std::string name = fontname(gc->fontfamily, gc->fontface, gd->system_aliases, gd->user_aliases);
  gdtools::context_set_font(gd->cc, name, gc->cex * gc->ps, is_bold(gc->fontface), is_italic(gc->fontface), file);
  FontMetric fm = gdtools::context_extents(gd->cc, std::string(str));

  return fm.width;
  return(0);
  VOID_END_RCPP
  return(0);

}

void image_metric_info(int c, pGEcontext gc, double* ascent, double* descent,
                       double* width, pDevDesc dd) {

  Rcout << "// image_metric_info()" << std::endl;
  BEGIN_RCPP
  GraffleDevice *gd = (GraffleDevice *)dd->deviceSpecific;

  bool is_unicode = mbcslocale;
  if (c < 0) {
    is_unicode = true;
    c = -c;
  }

  // Convert to string - negative implies unicode code point
  char str[16];
  if (is_unicode) {
    Rf_ucstoutf8(str, (unsigned int) c);
  } else {
    str[0] = (char) c;
    str[1] = '\0';
  }

  std::string file = fontfile(gc->fontfamily, gc->fontface, gd->user_aliases);
  std::string name = fontname(gc->fontfamily, gc->fontface, gd->system_aliases, gd->user_aliases);
  gdtools::context_set_font(gd->cc, name, gc->cex * gc->ps, is_bold(gc->fontface), is_italic(gc->fontface), file);
  FontMetric fm = gdtools::context_extents(gd->cc, std::string(str));

  *ascent = fm.ascent;
  *descent = fm.descent;
  *width = fm.width;
  VOID_END_RCPP

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
  double multiplier = 1/dd->ipr[0]/72;
  double lwd = gc->lwd * xlwd * multiplier;
  GraffleDevice *gd = (GraffleDevice *)dd->deviceSpecific;
  std::string new_line = tfm::format(
    "l1 = cnvs.addLine(%s, %s);\n",
    tfm::format("new Point(%f, %f)", d2s(x1), d2s(y1)),
    tfm::format("new Point(%f, %f)", d2s(x2), d2s(y2))
  );
  new_line = new_line + "l1.shadowColor = null;\n";
  new_line = new_line + tfm::format("l1.strokeThickness = %f;\n", lwd);
  new_line += tfm::format("l1.strokeColor = Color.RGB(%f, %f, %f, %f);",
                          R_RED(gc->col)/255.0, R_GREEN(gc->col)/255.0, R_BLUE(gc->col)/255.0,
                          R_ALPHA(gc->col)/255.0);
  Rcout << new_line << std::endl;
  VOID_END_RCPP

}

static void image_polyline(int n, double *x, double *y, const pGEcontext gc, pDevDesc dd) {

  Rcout << "// image_polyline()" << std::endl;
  BEGIN_RCPP
  double multiplier = 1/dd->ipr[0]/72;
  double lwd = gc->lwd * xlwd * multiplier;

  std::string new_line = "l1 = cnvs.newLine();\n";
  new_line = new_line + "l1.points = [\n  ";
  for (int i=0; i<n; i++) {
    new_line = new_line + tfm::format("new Point(%f, %f)", d2s(x[i]), d2s(y[i]));
    if (i < (n-1)) new_line = new_line + ", ";
  }
  new_line = new_line + "\n];\n";
  new_line = new_line + tfm::format("l1.strokeThickness = %f;\n", lwd);
  new_line = new_line + "l1.shadowColor = null;\n";
  new_line += tfm::format("l1.strokeColor = Color.RGB(%f, %f, %f, %f);",
                          R_RED(gc->col)/255.0, R_GREEN(gc->col)/255.0, R_BLUE(gc->col)/255.0,
                          R_ALPHA(gc->col)/255.0);
  Rcout << new_line << std::endl;
  VOID_END_RCPP

}

static void image_polygon(int n, double *x, double *y, const pGEcontext gc, pDevDesc dd) {

  Rcout << "// image_polygon()" << std::endl;
  BEGIN_RCPP
  double multiplier = 1/dd->ipr[0]/72;
  double lwd = gc->lwd * xlwd * multiplier;

  std::string new_gon = "g1 = cnvs.newShape();\n";
  double minx = x[0], maxx = x[0];
  double miny = y[0], maxy = y[0];

  for (int i=0; i<n; i++) {
    if (minx < x[i]) minx = x[i];
    if (maxx > x[i]) maxx = x[i];
    if (miny < y[i]) miny = y[i];
    if (maxy > y[i]) maxy = y[i];
  }

  new_gon = new_gon +
    tfm::format("g1.geometry = new Rect(%f, %f, %f, %f);\n", minx, miny, maxx, maxy);

  new_gon = new_gon + "g1.shapeVertices = [\n  ";
  for (int i=0; i<n; i++) {
    new_gon = new_gon + tfm::format("new Point(%f, %f)", d2s(x[i]), d2s(y[i]));
    if (i < (n-1)) new_gon = new_gon + ", ";
  }
  new_gon = new_gon + "\n];\n";
  new_gon = new_gon + "g1.fillType = null;\n";
  new_gon += tfm::format("g1.strokeColor = Color.RGB(%f, %f, %f, %f);\n",
                          R_RED(gc->col)/255.0, R_GREEN(gc->col)/255.0, R_BLUE(gc->col)/255.0,
                          R_ALPHA(gc->col)/255.0);
  new_gon += tfm::format("g1.fillColor = Color.RGB(%f, %f, %f, %f);\n",
                          R_RED(gc->fill)/255.0, R_GREEN(gc->fill)/255.0, R_BLUE(gc->fill)/255.0,
                          R_ALPHA(gc->fill)/255.0);

  Rcout << new_gon << std::endl;
  VOID_END_RCPP
}

static inline std::string fontname(const pGEcontext gc){
  // Symbols from text(font = 5) are a special case.
  // Windows: "Standard Symbols L" does NOT work with IM. Just use "Symbol".
  if(is_symbol(gc->fontface))
    return std::string("Symbol");
  return normalize_font(gc->fontfamily);
}

void image_text(double x, double y, const char *str, double rot, double hadj,
                pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_text()" << std::endl;
  BEGIN_RCPP
  double multiplier = 1/dd->ipr[0]/72;
  double deg = fmod(-rot + 360.0, 360.0);
  double ps = gc->ps * gc->cex * multiplier;

  GraffleDevice *gd = (GraffleDevice *)dd->deviceSpecific;
  std::string new_text = "t1 = ";
  new_text = new_text + "cnvs.addText(\"" + std::string(str) + "\", " +
    "new Point(" + d2s(x) + "," + d2s(y) + ")" + ");\n";
  new_text = new_text + "t1.shadowColor = null;\n";
  new_text = new_text + "t1.textSize = " + d2s(ps) + ";\n";
  new_text = new_text + "t1.fontName = \"" + fontname(gc) + "\";\n";
  new_text = new_text + "t1.textRotation = " + d2s(rot) + ";\n";
  new_text += tfm::format("t1.textColor = Color.RGB(%f, %f, %f, %f);\n",
                          R_RED(gc->col)/255.0, R_GREEN(gc->col)/255.0, R_BLUE(gc->col)/255.0,
                          R_ALPHA(gc->col)/255.0);
  new_text += tfm::format("t1.fillColor = Color.RGB(%f, %f, %f, %f);\n",
                          R_RED(gc->fill)/255.0, R_GREEN(gc->fill)/255.0, R_BLUE(gc->fill)/255.0,
                          R_ALPHA(gc->fill)/255.0);
  Rcout << new_text << std::endl;
  VOID_END_RCPP
}

static void image_rect(double x0, double y0, double x1, double y1,
                       const pGEcontext gc, pDevDesc dd) {
  Rcout << "// image_rect()" << std::endl;
  BEGIN_RCPP
  std::string new_rect = "r1 = cnvs.newShape();\n";
  new_rect += "r1.shape = \"Rectangle\";\n";
  new_rect += "r1.shadowColor = null;\n";
  new_rect += tfm::format("r1.geometry = new Rect(%f, %f, %f, %f);\n",
                          d2s(x0), d2s(y0), d2s(x1-x0), d2s(y0-y1));
  new_rect += tfm::format("r1.strokeColor = Color.RGB(%f, %f, %f, %f);\n",
                          R_RED(gc->col)/255.0, R_GREEN(gc->col)/255.0, R_BLUE(gc->col)/255.0,
                          R_ALPHA(gc->col)/255.0);
  new_rect += tfm::format("r1.fillColor = Color.RGB(%f, %f, %f, %f);\n",
                          R_RED(gc->fill)/255.0, R_GREEN(gc->fill)/255.0, R_BLUE(gc->fill)/255.0,
                          R_ALPHA(gc->fill)/255.0);
  Rcout << new_rect << std::endl;
  VOID_END_RCPP
}

static void image_circle(double x, double y, double r, const pGEcontext gc, pDevDesc dd) {

  Rcout << "// image_circle()" << std::endl;
  BEGIN_RCPP
  //note: parameter 3 + 4 must denote any point on the circle
  std::string new_circle = "c1 = cnvs.newShape();\n";
  new_circle += "c1.shape = \"Circle\";\n";
  new_circle += "c1.shadowColor = null;\n";
  new_circle += tfm::format("c1.geometry = new Rect(%f, %f, %f, %f);\n",
                            d2s(x-r), d2s(y-r), d2s(2*r), d2s(2*r));
  new_circle += tfm::format("c1.strokeColor = Color.RGB(%f, %f, %f, %f);\n",
                            R_RED(gc->col)/255.0, R_GREEN(gc->col)/255.0, R_BLUE(gc->col)/255.0,
                            R_ALPHA(gc->col)/255.0);
  new_circle += tfm::format("c1.fillColor = Color.RGB(%f, %f, %f, %f);\n",
                            R_RED(gc->fill)/255.0, R_GREEN(gc->fill)/255.0, R_BLUE(gc->fill)/255.0,
                            R_ALPHA(gc->fill)/255.0);
  Rcout << new_circle << std::endl;
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
                                   double ps, int res, int canclip, Rcpp::List &aliases) {

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
                        double pointsize, int res, bool canclip, Rcpp::List& aliases) {

  int bg = R_GE_str2col(bg_.c_str());

  R_GE_checkVersionOrDie(R_GE_version);
  R_CheckDeviceAvailable();

  BEGIN_SUSPEND_INTERRUPTS {

    pDevDesc dev = graffle_driver_new(device, bg, width, height, pointsize, res, canclip, aliases);

    if (dev == NULL)  throw std::runtime_error("Failed to start graffle device");

    pGEDevDesc dd = GEcreateDevDesc(dev);

    GEaddDevice2(dd, "graffle");

    GEinitDisplayList(dd);

  } END_SUSPEND_INTERRUPTS;

}

// [[Rcpp::export]]
std::string graffle_device_internal(std::string bg, int width, int height, double pointsize,
                                    int res, bool clip, bool antialias, bool drawing,
                                    Rcpp::List aliases) {

  GraffleDevice *device = new GraffleDevice(width, height, aliases);

  make_device(device, bg, width, height, pointsize, res, clip, aliases);

  return "";

}
