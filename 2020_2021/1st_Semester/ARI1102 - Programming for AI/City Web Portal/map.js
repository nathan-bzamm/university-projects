var mymap = L.map('mapid').setView([45.0703, 7.75], 10);
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(mymap);
var marker = L.marker([45.0703, 7.6869]).addTo(mymap);
