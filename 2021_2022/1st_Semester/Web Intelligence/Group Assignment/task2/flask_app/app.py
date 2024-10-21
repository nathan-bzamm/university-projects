from flask import Flask, json
from flask import render_template

app  = Flask(__name__)

@app.route('/jsondata')
def index():
    with app.open_resource('topKeyWords.json') as fp:
        data = json.load(fp)
    return render_template('json_data.html',data=data)