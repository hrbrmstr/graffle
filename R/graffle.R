#' Graphics device for OmniGraffle documents
#'
#' NOTE: OmniGraffle needs to be open and have a document open.
#'
#' @param width width
#' @param height height
#' @param bg bg
#' @param pointsize pointsize
#' @param res res
#' @param clip clip
#' @param antialias antialias
#' @param system_fonts system_fonts
#' @param user_fonts user_fonts
#' @export
#' @examples \dontrun{
#' graffle()
#' graffle()
#' plot(
#'   cars, main = "Stopping Distance versus Speed",
#'   col = "red", cex = 3
#' )
#' lines(stats::lowess(cars), col = "blue")
#' dev.off()
#'
#' graffle()
#' ggplot(mpg, aes(displ, hwy)) +
#'   geom_point(aes(color = drv)) +
#'   facet_wrap(vars(cyl, drv))
#' dev.off()
#' }
graffle <- function(width = 800, height = 600, bg = "white", pointsize = 12,
                       res = 72, clip = FALSE, antialias = FALSE,
                       system_fonts = list(), user_fonts = list()) {

  aliases <- validate_aliases(system_fonts, user_fonts)

  graffle_device_internal(
    bg = bg,
    width = width,
    height = height,
    pointsize = pointsize,
    res = res,
    clip = clip,
    antialias = antialias,
    drawing = FALSE,
    aliases = aliases
  ) -> graffle_script

}