document$.subscribe(function () {
  var el = document.getElementById("copyright-year")
  if (el) {
    el.textContent = new Date().getFullYear()
  }
})
