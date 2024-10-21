function getWeatherDetails ()
{
  var weatherRequest = new XMLHttpRequest ();
  var hpath = "http://api.weatherapi.com/v1/current.json?key=4ebb867608ea4c568ce180536202612&q=Turin";
  var jsonWeather;

  weatherRequest.open ("GET", hpath, false);
  weatherRequest.send ();

  console.log (weatherRequest.responseText);

  jsonWeather = JSON.parse (weatherRequest.responseText.trim());

  console.log (jsonWeather);

  outputWeatherDetails (jsonWeather);
}

function outputWeatherDetails (jsonWeather)
{
  var loc = jsonWeather.location.name + "<br />";
  document.getElementById("loc").innerHTML = loc;

  var con = jsonWeather.current.condition.text + " <br />";
  document.getElementById("con").innerHTML = con;

  var temp = jsonWeather.current.temp_c + "&#8451" + " <br />";
  document.getElementById("temp").innerHTML = temp;

  var date = jsonWeather.forecast.forecastday[0].date + " <br />";
  document.getElementById("date").innerHTML = date;

  var maxtemp = "Max " + jsonWeather.forecast.forecastday[0].day.maxtemp_c + "&#8451" + " <br />";
  document.getElementById("maxtemp").innerHTML = maxtemp;

  var mintemp = "Min " + jsonWeather.forecast.forecastday[0].day.mintemp_c + "&#8451" + " <br />";
  document.getElementById("mintemp").innerHTML = mintemp;
}
