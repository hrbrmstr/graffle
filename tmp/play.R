library(graffle)
library(ggplot2)
library(hrbrthemes)

capture.output({
  to_graffle()
  plot(
    cars, main = "Stopping Distance versus Speed",
    col = "red", ced = 3
  )
  lines(stats::lowess(cars), col = "blue")
  invisible(dev.off())
}) -> dbg

cat(dbg, file="~/Desktop/a.js", sep="\n")

browseURL("omnigraffle:///omnijs-run?script=c1%20%3D%20cnvs%2EnewShape%28%29%3B%0Ac1%2Eshape%20%3D%20%22Circle%22%3B%0Ac1%2EshadowColor%20%3D%20null%3B%0Ac1%2Egeometry%20%3D%20new%20Rect%2882%2E663%2C%20506%2E544%2C%205%2E4%2C%205%2E4%29%3B%0Ac1%2EstrokeColor%20%3D%20Color%2ERGB%280%2E000000%2C%200%2E000000%2C%200%2E000000%2C%201%2E000000%29%3B%0Ac1%2EfillColor%20%3D%20Color%2ERGB%281%2E000000%2C%201%2E000000%2C%201%2E000000%2C%200%2E000000%29%3B%0A")
cat(URLdecode("omnigraffle:///omnijs-run?script=c1%20%3D%20cnvs%2EnewShape%28%29%3B%0Ac1%2Eshape%20%3D%20%22Circle%22%3B%0Ac1%2EshadowColor%20%3D%20null%3B%0Ac1%2Egeometry%20%3D%20new%20Rect%2882%2E663%2C%20506%2E544%2C%205%2E4%2C%205%2E4%29%3B%0Ac1%2EstrokeColor%20%3D%20Color%2ERGB%280%2E000000%2C%200%2E000000%2C%200%2E000000%2C%201%2E000000%29%3B%0Ac1%2EfillColor%20%3D%20Color%2ERGB%281%2E000000%2C%201%2E000000%2C%201%2E000000%2C%200%2E000000%29%3B%0A"))
browseURL("omnigraffle:///omnijs-run?script=Document%2EmakeNewAndShow%28function%28doc%29%7B%0A%09cnvs%20%3D%20doc%2Ewindows%5B0%5D%2Eselection%2Ecanvas%0A%09cnvs%2EcanvasSizingMode%20%3D%20CanvasSizingMode%2EFixed%0A%09cnvs%2Esize%20%3D%20new%20Size%28612%2C792%29%0A%09cnvs%2Ename%20%3D%20%22Letter%20to%20Sally%22%0A%7D%29")

capture.output({
  to_graffle()
  plot(x <- sort(rnorm(47)), type = "s", main = "plot(x, type = 's')")
  points(x, cex = 2, col = "dark red")
  invisible(dev.off())
}) -> x
cat(x, file="~/Desktop/a.js", sep="\n")


capture.output({
  to_graffle()
  ggplot(mtcars, aes(wt, mpg)) +
    geom_point(aes(color=factor(cyl)), size=3) +
    geom_smooth() +
    labs(
      title="The Seminal ggplot2 example",
      subtitle="A useful subtitle",
      caption="For Omnigraffle"
    ) +
    scale_color_ipsum() +
    theme_ipsum_rc(grid="XY") -> gg
  print(gg)
  invisible(dev.off())
}) -> x
cat(x, file="~/Desktop/a.js", sep="\n")


capture.output({
  to_graffle()
  ggplot(economics_long, aes(date, value01, colour = variable)) +
    geom_line() +
    theme_ipsum_rc(grid="XY") -> gg
  print(gg)
  invisible(dev.off())
}) -> x
cat(x, file="~/Desktop/a.js", sep="\n")


capture.output({
  to_graffle()
  df <- data.frame(
    x = rep(c(2, 5, 7, 9, 12), 2),
    y = rep(c(1, 2), each = 5),
    z = factor(rep(1:5, each = 2)),
    w = rep(diff(c(0, 4, 6, 8, 10, 14)), 2)
  )
  ggplot(df, aes(xmin = x - w / 2, xmax = x + w / 2, ymin = y, ymax = y + 1)) +
    geom_rect(aes(fill = z, width = w), colour = "grey50") -> gg
  print(gg)
  invisible(dev.off())
}) -> x
cat(x, file="~/Desktop/a.js", sep="\n")

