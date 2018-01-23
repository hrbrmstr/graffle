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
  plot(x <- sort(rnorm(47)), type = "s", main = "plot(x, type = \"s\")")
  points(x, cex = .5, col = "dark red")
  invisible(dev.off())
}) -> x
cat(x, file="~/Desktop/a.js", sep="\n")


capture.output({
  to_graffle()
  ggplot(mtcars, aes(wt, mpg)) +
    geom_point() +
    geom_smooth() +
    labs(
      title="The Seminal ggplot2 example",
      subtitle="A useful subtitle",
      caption="For Omnigraffle"
    ) +
    theme_ipsum_rc(grid="XY") -> gg
  print(gg)
  invisible(dev.off())
}) -> x
cat(x, file="~/Desktop/a.js", sep="\n")
