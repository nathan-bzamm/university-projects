#!/usr/bin/python3

import cgi;
import json;
import nltk;
import langdetect;
from nltk import tokenize;
from langdetect import detect;
from nltk.tokenize import word_tokenize;


def getPostDataAsJson (environ):
    post_json = {};
    storage = cgi.FieldStorage (fp=environ['wsgi.input'], environ=environ, keep_blank_values=True);
    for k in storage.keys():
        post_json[k] = storage.getvalue(k);

    return (post_json);


def application (environ, start_response):
    form_params = getPostDataAsJson (environ);

    text = form_params["field"];
    english = list();
    error = list();
    try:
        if detect(text) == "en":
                english.append("True");
    except:
                error.append("Error!!");

    tokens = nltk.sent_tokenize(text);
    tokens = nltk.word_tokenize(text);

    POS1 = nltk.pos_tag(tokens, tagset = 'universal');
    POS2 = nltk.pos_tag(tokens);

    Names = nltk.chunk.tree2conlltags(nltk.ne_chunk(POS2));

    data = {}
    data = {
        "english":english,
        "error":error,
        "POS1":POS1,
        "POS2":POS2,
        "Names":Names
    };


    #Adding strings to the response body
    conttype = 'application/json';
    response_body = [bytes(json.dumps (data), encoding='utf-8')];

    #find the content-length (sum of all strings' lengths)
    content_length = sum([len(s) for s in response_body]);

    status = '200 OK';
    response_header = [('Content-type',conttype), ('Content-Length', str(content_length))];

    start_response (status, response_header);
    return response_body;
