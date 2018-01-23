library(graffle)

capture.output({
  to_graffle()
  plot(
    cars, main = "Stopping Distance versus Speed",
    col = "red", ced = 3
  )
  lines(stats::lowess(cars), col = "blue")
  invisible(dev.off())
}) -> dbg
