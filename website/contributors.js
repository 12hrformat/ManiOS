/* ============================================
   ManiOS — Contributors
   Clean list of names + GitHub links
   ============================================ */

(function () {
  // ---- Contributors ----
  const CONTRIBUTORS = [
    { login: "12hrformat", html_url: "https://github.com/12hrformat" },
    { login: "firstspot7", html_url: "https://github.com/firstspot7" },
    { login: "farcrowx", html_url: "https://github.com/farcrowx" },
    { login: "krishnarajyagru27-creator", html_url: "https://github.com/krishnarajyagru27-creator" },
  ];

  // ---- Helpers ----
  function $(id) { return document.getElementById(id); }

  function renderCards(data, containerId) {
    const grid = $(containerId);
    if (!grid) return;
    grid.innerHTML = "";

    data.forEach(function (c) {
      const card = document.createElement("div");
      card.className = "card";
      card.innerHTML =
        '<a href="' + c.html_url + '" style="color:inherit;text-decoration:none;">' +
        '<h3>' + c.login + '</h3>' +
        '</a>';
      grid.appendChild(card);
    });
  }

  function init() {
    renderCards(CONTRIBUTORS, "home-contributors");
    renderCards(CONTRIBUTORS, "contributors-grid");
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init);
  } else {
    init();
  }
})();
