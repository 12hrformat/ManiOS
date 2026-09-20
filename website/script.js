/* ============================================
   ManiOS — JavaScript
   Mobile nav, active nav link
   ============================================ */

document.addEventListener('DOMContentLoaded', () => {
  // --- Mobile Nav Toggle ---
  const menuBtn = document.getElementById('menuBtn');
  const navLinks = document.getElementById('navLinks');

  if (menuBtn && navLinks) {
    menuBtn.addEventListener('click', () => {
      navLinks.classList.toggle('open');
    });
  }

  // --- Active Nav Link on Scroll ---
  const sections = document.querySelectorAll('section');
  const navAnchors = document.querySelectorAll('.nav-links a');

  function updateActiveNav() {
    let current = '';
    sections.forEach((section) => {
      const sectionTop = section.offsetTop - 80;
      if (window.scrollY >= sectionTop) {
        current = section.getAttribute('id') || '';
      }
    });

    navAnchors.forEach((a) => {
      a.classList.remove('active');
      if (a.getAttribute('href') === '#' + current) {
        a.classList.add('active');
      }
    });
  }

  window.addEventListener('scroll', updateActiveNav);

  // Close mobile nav on link click
  navAnchors.forEach((a) => {
    a.addEventListener('click', () => {
      navLinks.classList.remove('open');
    });
  });
});
