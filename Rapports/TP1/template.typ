#let print_title(title) = {
  align(center, text(20pt)[
    *#title*
  ])
}

#let template(doc) = {
  set par(justify: true)
  set page(numbering: "1")
  set page(paper: "a4")

  show link: underline
  show link: set text(navy)

  set raw(lang: "cpp")
  set raw(tab-size: 2)

  set heading(numbering: "1.1")

  // Display inline code in a small box
  // that retains the correct baseline.
  show raw.where(block: false): box.with(
    fill: luma(244),
    inset: (x: 3pt, y: 0pt),
    outset: (y: 2pt),
    radius: 2pt,
  )

  // Display block code in a larger block
  // with more padding.
  show raw.where(block: true): block.with(
    fill: luma(244),
    inset: 2pt,
    outset: 2pt,
    radius: 2pt,
    width: 100%,
  )

  // Don't forget this
  doc
}
