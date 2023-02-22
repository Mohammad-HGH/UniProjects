import pandas as pd
import sqlite3
from .word import Word, Comment
import re


(DEFAULT_DATABASE_FILENAME, TABLE_COMMENTS, TABLE_POSTS, COL_TEXT, COL_ID, COL_POST_ID, COL_OWNER, COL_DATE, COL_LIKES, COL_LOCATION, COL_SCORE ) \
    = ('models/database.db', 'comments', 'posts', 'text', 'id', 'post_id', 'owner', 'date', 'likes', 'location', 'score')

def load_excel(filename='models/avocado.csv'):
    data = pd.read_csv(filename)
    data = data.query('type == "conventional" and region == "Albany"')
    return data


def dict_factory(cursor, row):
    dict_result = {}
    for index, column in enumerate(cursor.description):
        dict_result[column[0]] = row[index]
    return dict_result


def db_loadall(database=DEFAULT_DATABASE_FILENAME):
    # load all tables
     connection = sqlite3.connect(database)
     connection.row_factory = dict_factory
     cursor = connection.cursor()
     cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
     for table in cursor.fetchall():
        yield list(cursor.execute('SELECT * from ?;', (table[0],)))


def db_load(database=DEFAULT_DATABASE_FILENAME, table_name = TABLE_COMMENTS, queries=[]):
    connection = sqlite3.connect(database)
    connection.row_factory = dict_factory
    cursor = connection.cursor()
    return list(cursor.execute(f"SELECT * FROM {table_name}"))


def extract_words(database):
    Word.S = []  # clear previous words
    for row in database:
        id = row[COL_ID]
        text = row[COL_TEXT]
        post_id = row[COL_POST_ID]
        owner  = row[COL_OWNER]
        date = row[COL_DATE]
        likes = row[COL_LIKES]
        score = row[COL_SCORE]
        Comment(text, post_id, owner, likes, score, date)
        # split words, delimiter: any non alphabetic character except #
        words = re.split("[^a-zA-Z#]", text)
        # remove reduncdants:
        words = filter(lambda s: s, words)

        #filter signs
        for word in words:
            Word(id, post_id, word, owner, date, likes, score)
        # Word.S will be automatically loaded with words


