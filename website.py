import json

import pandas as pd
from flask import Flask
from flask import render_template, request

from algorithm.keyword.keywords import get_keyword, clean_words, remove_stop, english_stop_words

app = Flask(__name__)


@app.route('/')
def index():
    return render_template('index1.html')


@app.route('/ajax/')
def ajax():
    return render_template('index1.html')


@app.route('/add_numbers/', methods=['POST'])
def add_numbers():
    text = request.form['file']
    clean = clean_words(text)
    # print(clean)
    result = get_keyword(remove_stop(clean))
    result = dict(result[:30])  # 因为取了钱30可以忽略负数值问题
    # print(result)
    v = pd.cut([abs(x) for x in list(result.values())], 5, retbins=True, labels=[1, 2, 3, 5, 10])[0]
    show1 = ""
    result = dict(zip(result.keys(), v))
    for key, value in result.items():
        print(key, value)
        show1 += "<span data-weight=\" " + str(value) + " \">" + key + "</span>";

    show2 = ""
    for word in clean:
        if word in english_stop_words:
            show2 += "<span class=\"label label-w00\">" + word + "</span>";
        elif word in result.keys():
            if result[word] != 1:
                show2 += "<span class=\"label label-w" + str(result[word]) + "\">" + word + "</span>";
        else:
            show2 += "<span class=\"label label-w0\">" + word + "</span>";

    return json.dumps({'v1': show1, 'v2': show2})


@app.route('/search', methods=['POST'])
def search():
    print(request.form['article'])
    return 0


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=False)
