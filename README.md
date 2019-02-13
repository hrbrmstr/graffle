
# graffle

An ‘OmniGraffle’ Graphics Device \[WIP\]

## Description

A graphics device that produces ‘OmniGraffle’ documents. Currently based
on the code structure of the ‘svglite’ package.

## What’s Inside The Tin

The following functions are implemented:

  - `graffle`: A graphics device that produces ‘OmniGraffle’ documents

## NOTE

This is a WIP but it may still be useful to some folks. The pacakge
(perhaps somewhat obviously) requires that you have OmniGraffle
installed.

It also requires that you have it open and — for the moment — have at
least one document window open. It relies on the [Omni Automation
API](https://omni-automation.com/omnigraffle/) and “draws” directly in a
new document.

Eventually it will not be necessary to have OmniGraffle already open or
also have a document open to make this work.

Since it’s instrumenting an application to draw, it’s also not
super-speedy. Yay for closed document formats?

## TODO

  - Fix need to have OG up and an existing or new doc open
  - Text/fonts
  - Tests

## Installation

``` r
devtools::install_git("https://git.sr.ht/~hrbrmstr/graffle")
# OR 
devtools::install_gitlab("hrbrmstr/graffle")
# OF 
devtools::install_github("hrbrmstr/graffle")
```

## Usage

``` r
library(graffle)

# current verison
packageVersion("graffle")
```

    ## [1] '0.1.0'

### Base graphics

``` r
graffle()
plot(
  cars, main = "Stopping Distance versus Speed",
  col = "red", cex = 3
)
lines(stats::lowess(cars), col = "blue")
dev.off()
```

![](man/figures/base-graphics.gif)

### ggplot2

``` r
graffle()
ggplot(mpg, aes(displ, hwy)) +
  geom_point(aes(color = drv)) +
  facet_wrap(vars(cyl, drv))
dev.off()
```

![](man/figures/ggplot2-example.gif)

## `graffle` Metrics

| Lang | \# Files |  (%) | LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
| :--- | -------: | ---: | --: | ---: | ----------: | ---: | -------: | ---: |
| C++  |        3 | 0.23 | 530 | 0.62 |         125 | 0.62 |       65 | 0.42 |
| R    |        9 | 0.69 | 303 | 0.36 |          47 | 0.23 |       46 | 0.30 |
| Rmd  |        1 | 0.08 |  19 | 0.02 |          31 | 0.15 |       44 | 0.28 |

## Code of Conduct

Please note that the ‘graffle’ project is released with a [Contributor
Code of Conduct](CODE_OF_CONDUCT.md). By contributing to this project,
you agree to abide by its terms.
