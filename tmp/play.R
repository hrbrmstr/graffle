library(graffle)
library(ggplot2)
library(hrbrthemes)

capture.output({
  to_graffle()
  plot(cars, main = "Stopping Distance versus Speed")
  lines(stats::lowess(cars))
  invisible(dev.off())
}) -> x
cat(x, file="~/Desktop/a.js", sep="\n")


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

