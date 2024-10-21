import requests
import json

searchTerm = "parrot"
startIndex = 1
api_key = "AIzaSyCtn8vVqpXzAbR5le2aoNrRr9hzktVU7zA"
cx = 'c485268cc94e646db'
searchUrl = "https://www.googleapis.com/customsearch/v1?q=" + \
    searchTerm + "&start=" + startIndex + "&key=" + api_key + "&cx=" + cx + \
    "&searchType=image"

r = requests.get(searchUrl)
response = r.content.decode('utf-8')
result = json.loads(response)
print(searchUrl)
print(r)
print(result)