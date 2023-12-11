from celery import Celery, shared_task
#My rabbitmq url http://localhost:15672/#/
from time import sleep

app = Celery('tasks', broker='amqp://localhost', backend='db+sqlite:///db.sqlite3')
#app.conf.CELERY_RESULT_BACKEND = 'db+sqlite://results/sqlite'

@app.task
def reverse(text):
    return text[::-1]