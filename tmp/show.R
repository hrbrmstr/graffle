library(graffle)
library(ggplot2)

us_map <- map_data("state")
capture.output({
  to_graffle()
  ggplot(us_map, aes(long, lat)) +
    geom_map(map = us_map, aes(map_id=region, fill=order), color="#b2b2b2", size=0.25) +
    coord_map("polyconic") +
    viridis::scale_fill_viridis() +
    ggthemes::theme_map() +
    theme(legend.position="none") -> gg
  print(gg)
  invisible(dev.off())
}) -> dbg
