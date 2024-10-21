from googleapiclient.discovery import build


api_key = "AIzaSyCtn8vVqpXzAbR5le2aoNrRr9hzktVU7zA"
cse_key = 'c485268cc94e646db'

resource = build("customsearch", 'v1', developerKey=api_key).cse()
result = resource.list(q = 'parrot', cx = cse_key, searchType = 'image').execute()

for item in result['items']:
    print(item['title'], item['link'])