function showPage(pageId) {
    document.querySelectorAll('.page-container').forEach(page => {
        page.style.display = 'none';
    });
    document.getElementById(pageId).style.display = 'block';
    window.scrollTo(0, 0);
}

window.addEventListener('hashchange', () => {
    const page = window.location.hash.substring(1);
    showPage(page || 'home');
});

document.addEventListener('DOMContentLoaded', () => {
    const initialPage = window.location.hash.substring(1) || 'home';
    showPage(initialPage);
});

document.querySelectorAll('.nav-links a').forEach(link => {
    link.addEventListener('click', (e) => {
        e.preventDefault();
        const page = link.getAttribute('href').substring(1);
        window.location.hash = page;
    });
});