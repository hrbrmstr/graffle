#' @export
to_graffle <- function(width = 800, height = 600, bg = "white", pointsize = 12,
                       res = 72, clip = TRUE, antialias = TRUE,
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